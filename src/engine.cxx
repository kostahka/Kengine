#include "Kengine/engine.hxx"
#include "engine.hxx"

#ifdef __ANDROID__
 #include <SDL3/SDL_main.h>
#endif

#ifdef ENGINE_DEV
 #include <filesystem>
#endif

#include <SDL3/SDL.h>

#include "Kengine/configuration/configuration-file.hxx"
#include "Kengine/file-last-modify-listener.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/window/window.hxx"
#include "audio/audio.hxx"
#include "event/event.hxx"
#include "graphics/graphics.hxx"
#include "imgui/imgui.hxx"
#include "log/log.hxx"
#include "opengl/opengl.hxx"
#include "physics/physics.hxx"
#include "scene/scene-manager.hxx"
#include "window/window.hxx"

namespace Kengine
{
    configuration_file engine_settings("kengine");
    game*              e_game = nullptr;

    // Time from init SDL in milliseconds
    std::chrono::high_resolution_clock::time_point update_time;
    std::chrono::high_resolution_clock::time_point render_time;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point current_time;

    engine_configuration configuration{
        std::chrono::milliseconds{ 1000 / 60 },
        std::chrono::milliseconds{ 1000 / 90 },
    };

    bool initialize(const char* name)
    {
        log::initialize();

        auto sdl_init_flags = SDL_INIT_AUDIO | SDL_INIT_EVENTS |
                              SDL_INIT_GAMEPAD | SDL_INIT_VIDEO |
                              SDL_INIT_JOYSTICK | SDL_INIT_SENSOR;

        if (SDL_Init(sdl_init_flags) < 0)
        {
            KENGINE_FATAL("Error to initialize SDL. Error: {}", SDL_GetError());
            return false;
        }

        engine_settings.load();

        if (!window::initialize(name))
        {
            KENGINE_FATAL("Error to initialize window.");
            return false;
        }

        KENGINE_TRACE("Init engine systems...");
        audio::initialize();
        resource_manager::initialize();
        physics::initialize();
        scene_manager::initialize();

#ifdef KENGINE_IMGUI
        imgui::initialize();
#endif
        KENGINE_INFO("Assets folder: {}",
                     scene_manager::assets_base_folder.string().c_str());

        return true;
    };

    void shutdown()
    {
        imgui::shutdown();

        scene_manager::shutdown();
        physics::shutdown();
        audio::shutdown();
        resource_manager::shutdown();
        window::shutdown();

        engine_settings.save();

        SDL_Quit();

        log::shutdown();
    };

    void render(int delta_ms)
    {
        graphics::begin_render();

        e_game->get_current_scene().on_render(delta_ms);
        e_game->on_render(delta_ms);

#ifdef KENGINE_IMGUI
        imgui::draw();
#endif
        graphics::end_render();
    }

    void update(int delta_ms)
    {
        e_game->get_current_scene().on_update(delta_ms);
        e_game->on_update(delta_ms);
    }

    void start_game_loop()
    {
        KENGINE_ASSERT(e_game, "Game is not set");

        bool continue_loop = true;
        start_time = render_time = update_time =
            std::chrono::high_resolution_clock::now();

        file_last_modify_listener::get_instance()->start_files_watch();

#ifdef KENGINE_IMGUI
        auto pf_imgui_render = e_game->get_imgui_render();
        if (pf_imgui_render)
            imgui::registrate_imgui_render(pf_imgui_render);
#endif

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
        }
        else

        {
            failure = SDL_HideCursor();
        }

        if (failure)
        {
            KENGINE_ERROR("Error to set cursor visibility. Error: {}",
                          SDL_GetError());
        }
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

    bool run(create_game_func* pf_create_game, const char* name)
    {
        bool good = false;
        if (initialize(name))
        {
            e_game = pf_create_game();
            KENGINE_ASSERT(e_game, "Function doesn't return game pointer");
            if (e_game)
            {
                e_game->load_scene_links();
                start_game_loop();
                delete e_game;
                e_game = nullptr;
                good   = true;
            }

            shutdown();
        }

        return good;
    }

} // namespace Kengine
