#include "Kengine/engine.hxx"
#include "engine.hxx"

#include <iostream>
#include <streambuf>

#ifdef __ANDROID__
 #include <GLES3/gl3.h>
 #include <SDL3/SDL_main.h>
 #include <android/log.h>
#else
 #include <glad/glad.h>
#endif

#ifdef ENGINE_DEV
 #include "Kengine/file-last-modify-listener.hxx"
 #include <filesystem>
#endif

#include <SDL3/SDL.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include "Kengine/render/engine-resources.hxx"
#include "Kengine/window/window.hxx"
#include "audio/audio.hxx"
#include "event/event-engine.hxx"
#include "event/handle-user-event.hxx"
#include "render/opengl-error.hxx"
#include "window/window.hxx"

namespace Kengine
{

#ifdef ENGINE_DEV
    void reload_game(void *data);
#endif

    class android_redirected_buf : public std::streambuf
    {
    public:
        android_redirected_buf() = default;

    private:
        // This android_redirected_buf buffer has no buffer. So every character
        // "overflows" and can be put directly into the teed buffers.
        int overflow(int c) override
        {
            if (c == EOF)
            {
                return !EOF;
            }
            else
            {
                if (c == '\n')
                {
#ifdef __ANDROID__
                    // android log function add '\n' on every print itself
                    __android_log_print(
                        ANDROID_LOG_ERROR, "Kengine", "%s", message.c_str());
#else
                    std::printf("%s\n", message.c_str()); // TODO test only
#endif
                    message.clear();
                }
                else
                {
                    message.push_back(static_cast<char>(c));
                }
                return c;
            }
        }

        int sync() override { return 0; }

        std::string message;
    };

    game *e_game = nullptr;

#ifdef ENGINE_DEV
    std::string lib_name     = "";
    std::string tmp_lib_name = "";
    void       *lib_handle   = nullptr;
#endif

    // Time from init SDL in milliseconds
    std::chrono::high_resolution_clock::time_point update_time;
    std::chrono::high_resolution_clock::time_point render_time;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point current_time;

    std::basic_streambuf<char> *cout_buf{ nullptr };
    std::basic_streambuf<char> *cerr_buf{ nullptr };
    std::basic_streambuf<char> *clog_buf{ nullptr };

    engine_configuration configuration{
        std::chrono::milliseconds{ 1000 / 60 },
        std::chrono::milliseconds{ 1000 / 90 },
    };

    std::string_view initialize()
    {
        cout_buf = std::cout.rdbuf();
        cerr_buf = std::cerr.rdbuf();
        clog_buf = std::clog.rdbuf();

#ifdef __ANDROID__
        std::cout.rdbuf(&logcat);
        std::cerr.rdbuf(&logcat);
        std::clog.rdbuf(&logcat);
#endif

        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cerr << "Error to initialize SDL. Error: " << SDL_GetError()
                      << std::endl;
            return "sdl init fail";
        }

        window::initialize();

        std::cout << "Init engine resource..." << std::endl;
        e_resources::init();

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForOpenGL(window::get_sdl_window(),
                                     window::get_context());
        ImGui_ImplOpenGL3_Init("#version 300 es");

        audio::init();

        return "good";
    };

    std::string_view shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        window::shutdown();

        SDL_Quit();

        std::cout.rdbuf(cout_buf);
        std::cerr.rdbuf(cerr_buf);
        std::clog.rdbuf(clog_buf);

        return "good";
    };

    std::string_view start_game_loop()
    {
        if (e_game == nullptr)
            return "game not set";

        const auto w_pixels_size = window::get_size_in_pixels();
        glViewport(0, 0, w_pixels_size.x, w_pixels_size.y);
#ifdef ENGINE_DEV
        file_last_modify_listener::get_instance()->start_files_watch();
#endif
        bool continue_loop = true;

        e_game->on_start();

        start_time = render_time = update_time =
            std::chrono::high_resolution_clock::now();

        while (continue_loop)
        {
#ifdef ENGINE_DEV
            file_last_modify_listener::get_instance()
                ->handle_file_modify_listeners();
#endif
            continue_loop =
                event::poll_events(e_game, window::get_sdl_window());

            current_time = std::chrono::high_resolution_clock::now();

            auto update_delta_time = current_time - update_time;
            if (update_delta_time > configuration.update_delta_time)
            {
                const int delta_ms = static_cast<int>(
                    duration_cast<std::chrono::milliseconds>(update_delta_time)
                        .count());
                e_game->on_update(delta_ms);
                update_time = current_time;
            }
            auto render_delta_time = current_time - render_time;
            if (render_delta_time > configuration.render_delta_time)
            {
                const int delta_ms = static_cast<int>(
                    duration_cast<std::chrono::milliseconds>(render_delta_time)
                        .count());
                e_game->on_render(delta_ms);
                render_time = current_time;
            }
        }

        return "good";
    };

    void set_game(game *g)
    {
        e_game = g;
    }

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
            std::cerr << "Error to set cursor visibility. Error: "
                      << SDL_GetError() << std::endl;
        }
    };

    void clear_color(vec4 col)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gl_get_error(__LINE__, __FILE__);
        glClearColor(col.x, col.y, col.z, col.w);
        gl_get_error(__LINE__, __FILE__);
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

    void swap_buffers()
    {
        SDL_GL_SwapWindow(window::get_sdl_window());
        gl_get_error(__LINE__, __FILE__);
    };

    std::chrono::duration<int, std::milli> get_time()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - start_time);
    };

    void quit()
    {
        auto *quit = new SDL_Event();
        quit->type = SDL_EVENT_QUIT;
        SDL_PushEvent(quit);
    };

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
                std::cerr << "Failed to remove temp lib [" << tmp_lib_name
                          << "]" << std::endl;
                return false;
            }
        }

        try
        {
            copy_file(lib_name, tmp_lib_name);
        }
        catch (const std::exception *ex)
        {
            std::cerr << "Failed to copy from [" << lib_name << "] to ["
                      << tmp_lib_name << "]: " << ex->what() << std::endl;
            return false;
        }

        lib_handle = SDL_LoadObject(tmp_lib_name.c_str());

        if (lib_handle == nullptr)
        {
            std::cerr << "Failed to load lib from [" << tmp_lib_name
                      << "]. Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_FunctionPointer create_game_func_ptr =
            SDL_LoadFunction(lib_handle, "create_game");

        if (create_game_func_ptr == nullptr)
        {
            std::cerr << "Failed to load function [create_game] from ["
                      << tmp_lib_name << "]. Error: " << SDL_GetError()
                      << std::endl;
            return false;
        }

        using create_game_ptr = decltype(&create_game);

        create_game_ptr create_game_func =
            reinterpret_cast<create_game_ptr>(create_game_func_ptr);

        game *new_game = create_game_func();

        if (new_game == nullptr)
        {
            std::cerr << "Failed to create game" << std::endl;
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

        auto loop_return_code = start_game_loop();

        return loop_return_code;
    };

    bool reload_e_game()
    {
        bool result = load_e_game();
        if (result)
            e_game->on_start();
        return false;
    };

#endif

    android_redirected_buf logcat;

#ifdef ENGINE_DEV
    void reload_game(void *data)
    {
        reload_e_game();
    };
#endif

} // namespace Kengine

#ifdef ENGINE_DEV
int main(int argc, char *argv[])
{

    if (Kengine::initialize() != "good")
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
