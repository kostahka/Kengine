#include "Kengine/engine.hxx"
#include "engine.hxx"

#ifdef __ANDROID__
 #include <SDL3/SDL_main.h>
#endif

#ifdef ENGINE_DEV
 #include <filesystem>
#endif

#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "Kengine/file-last-modify-listener.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/render/engine-resources.hxx"
#include "Kengine/window/window.hxx"
#include "audio/audio.hxx"
#include "event/event.hxx"
#include "event/handle-user-event.hxx"
#include "graphics/render-manager.hxx"
#include "log/log.hxx"
#include "opengl/opengl.hxx"
#include "window/window.hxx"

namespace Kengine
{

#ifdef ENGINE_DEV
    void reload_game(void* data);
#endif

    game* e_game = nullptr;

#ifdef ENGINE_DEV
    std::string lib_name     = "";
    std::string tmp_lib_name = "";
    void*       lib_handle   = nullptr;
#endif

    // Time from init SDL in milliseconds
    std::chrono::high_resolution_clock::time_point update_time;
    std::chrono::high_resolution_clock::time_point render_time;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point current_time;

    engine_configuration configuration{
        std::chrono::milliseconds{ 1000 / 60 },
        std::chrono::milliseconds{ 1000 / 90 },
    };

    bool initialize()
    {
        log::initialize();

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            KENGINE_FATAL("Error to initialize SDL. Error: {}", SDL_GetError());
            return false;
        }

        if (!window::initialize())
        {
            KENGINE_FATAL("Error to initialize window.");
            return false;
        }

        KENGINE_TRACE("Init engine resource...");
        e_resources::init();

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForOpenGL(window::window, window::context);
        ImGui_ImplOpenGL3_Init("#version 300 es");

        // audio::init();

        return true;
    };

    void shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        window::shutdown();

        SDL_Quit();

        log::shutdown();
    };

    void render(int delta_ms)
    {
        graphics::render_manager::begin_render();
        e_game->on_render(delta_ms);
        graphics::render_manager::end_render();
    }

    void update(int delta_ms)
    {
        e_game->on_update(delta_ms);
    }

    void start_game_loop()
    {
        KENGINE_ASSERT(e_game, "Game is not set");

        bool continue_loop = true;
        start_time = render_time = update_time =
            std::chrono::high_resolution_clock::now();

        file_last_modify_listener::get_instance()->start_files_watch();
        e_game->on_start();

        while (continue_loop)
        {
            file_last_modify_listener::get_instance()
                ->handle_file_modify_listeners();
            continue_loop = event::poll_events(e_game);

            current_time = std::chrono::high_resolution_clock::now();

            auto update_delta_time = current_time - update_time;
            if (update_delta_time > configuration.update_delta_time)
            {
                const int delta_ms = static_cast<int>(
                    duration_cast<std::chrono::milliseconds>(update_delta_time)
                        .count());
                update(delta_ms);
                update_time = current_time;
            }
            auto render_delta_time = current_time - render_time;
            if (render_delta_time > configuration.render_delta_time)
            {
                const int delta_ms = static_cast<int>(
                    duration_cast<std::chrono::milliseconds>(render_delta_time)
                        .count());
                render(delta_ms);

                render_time = current_time;
            }
        }
    };

    void set_cursor_visible(bool visible)
    {
        int failure;
        if (visible)
        {
            failure = SDL_ShowCursor();
            ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
        }
        else

        {
            failure = SDL_HideCursor();
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        }

        if (failure)
        {
            KENGINE_ERROR("Error to set cursor visibility. Error: {}",
                          SDL_GetError());
        }
    };

    void draw_imgui()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        e_game->on_imgui_render();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    int get_time_ms()
    {
        return static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(current_time -
                                                                  start_time)
                .count());
    };

    void quit()
    {
        auto* quit = new SDL_Event();
        quit->type = SDL_EVENT_QUIT;
        SDL_PushEvent(quit);
    };

    bool run(create_game_func* pf_create_game)
    {
        bool good = false;
        if (initialize())
        {
            e_game = pf_create_game();
            KENGINE_ASSERT(e_game, "Function doesn't return game pointer");
            if (e_game)
            {
                start_game_loop();
                delete e_game;
                e_game = nullptr;
                good   = true;
            }

            shutdown();
        }

        return good;
    }

#ifdef ENGINE_DEV
    std::string_view dev_initialization(std::string lib_name,
                                        std::string tmp_lib_name)
    {
        lib_name     = lib_name;
        tmp_lib_name = tmp_lib_name;
        return "good";
    };

    bool load_e_game()
    {
        if (e_game)
        {
            delete e_game;
            SDL_UnloadObject(lib_handle);
        }

        using namespace std::filesystem;
        if (exists(tmp_lib_name))
        {
            if (!remove(tmp_lib_name))
            {
                KENGINE_ERROR("Failed to remove temp lib [{}]", tmp_lib_name);
                return false;
            }
        }

        try
        {
            copy_file(lib_name, tmp_lib_name);
        }
        catch (const std::exception* ex)
        {
            KENGINE_ERROR("Failed to copy from [{}] to [{}]: {}",
                          lib_name,
                          tmp_lib_name,
                          ex->what());
            return false;
        }

        lib_handle = SDL_LoadObject(tmp_lib_name.c_str());

        if (lib_handle == nullptr)
        {
            KENGINE_ERROR("Failed to load lib from [{}]. Error: {}",
                          tmp_lib_name,
                          SDL_GetError());
            return false;
        }

        SDL_FunctionPointer create_game_func_ptr =
            SDL_LoadFunction(lib_handle, "create_game");

        if (create_game_func_ptr == nullptr)
        {
            KENGINE_ERROR(
                "Failed to load function [create_game] from [{}]. Error: {}",
                tmp_lib_name,
                SDL_GetError());
            return false;
        }

        using create_game_ptr = decltype(&create_game);

        create_game_ptr create_game_func =
            reinterpret_cast<create_game_ptr>(create_game_func_ptr);

        game* new_game = create_game_func();

        if (new_game == nullptr)
        {
            KENGINE_ERROR("Failed to create game");
            return false;
        }

        e_game = new_game;

        return true;
    };

    std::string_view start_dev_game_loop()
    {
        if (lib_name == "" || tmp_lib_name == "")
            return "no dev init";

        load_e_game();

        auto file_listener = file_last_modify_listener::get_instance();
        file_listener->add_file(lib_name, &reload_game, nullptr);

        start_game_loop();

        return "good";
    };

    bool reload_e_game()
    {
        bool result = load_e_game();
        if (result)
            e_game->on_start();
        return false;
    };

#endif

#ifdef ENGINE_DEV
    void reload_game(void* data)
    {
        reload_e_game();
    };
#endif

} // namespace Kengine

#ifdef ENGINE_DEV
int main(int argc, char* argv[])
{

    if (!Kengine::initialize())
        return EXIT_FAILURE;
    using namespace std::string_literals;
    std::string game_name = ENGINE_GAME_LIB_NAME;
    std::string lib_name  = SDL_GetPlatform() == "Windows"s
                                ? game_name + ".dll"
                                : "./lib" + game_name + ".so";

    std::string tmp_lib_name = "./temp.dll";

    Kengine::dev_initialization(lib_name, tmp_lib_name);

    Kengine::start_dev_game_loop();
    Kengine::shutdown();

    return EXIT_SUCCESS;
};
#endif
