#include "window.hxx"

#include "SDL3/SDL_render.h"

#include "../graphics/graphics.hxx"
#include "../opengl/opengl-debug.hxx"
#include "Kengine/configuration/configuration-file.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/window/display.hxx"
#include "SDL3/SDL_video.h"
#include "display.hxx"

#ifdef __ANDROID__
 #define SDL_WINDOW_RESIZABLE 0
#endif

namespace Kengine::window
{
    ivec2         size{ 0, 0 };
    ivec2         size_in_pixels{ 0, 0 };
    bool          fullscreen = true;
    display::mode fullscreen_mode{ 0, 0, 0 };
    bool          maximized = false;
    bool          hidden    = false;

    Kengine::configuration override_configuration{};
    configuration_file     configuration("kengine_window");

    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;

    bool gl_debug = false;

    int gl_major_version = 3;
    int gl_minor_version = 0;
    int gl_profile       = SDL_GL_CONTEXT_PROFILE_ES;

    void pre_call_callback_gl(const char* name,
                              void*       funcptr,
                              int         len_args,
                              ...)
    {
        (void)name;
        (void)funcptr;
        (void)len_args;
    }

    void override_startup_options(const Kengine::configuration& override_conf)
    {
        override_configuration = override_conf;
    }

    void update_flags()
    {
        if (window)
        {
            Uint32 wnd_flags = SDL_GetWindowFlags(window);
            maximized        = wnd_flags & SDL_WINDOW_MAXIMIZED;
            hidden           = wnd_flags & SDL_WINDOW_HIDDEN;
        }
    }

    void update_sizes()
    {
        if (window)
        {
            SDL_GetWindowSize(window, &size.x, &size.y);
            SDL_GetWindowSizeInPixels(
                window, &size_in_pixels.x, &size_in_pixels.y);
        }
        if (context)
        {
            graphics::update_viewport();
        }
    }

    ivec2 get_size_in_pixels()
    {
        return size_in_pixels;
    }

    ivec2 get_size()
    {
        return size;
    }

    bool is_maximized()
    {
        return maximized;
    }

    bool is_hidden()
    {
        return hidden;
    }

    void set_size(ivec2& s)
    {
        SDL_SetWindowSize(window, s.x, s.y);
        update_sizes();
    }

    void set_fullscreen(bool is_fullscreen)
    {
        fullscreen = is_fullscreen;
        if (window)
            SDL_SetWindowFullscreen(window, is_fullscreen);
        graphics::update_viewport();
    }

    void raise()
    {
        if (window)
            SDL_RaiseWindow(window);
    }

    void update_window_fullscreen_mode()
    {
        SDL_DisplayMode sdl_mode{};
        bool            sdl_mode_success =
            display::get_closest_display_mode(fullscreen_mode, &sdl_mode);
        if (sdl_mode_success)
        {
            int failure = SDL_SetWindowFullscreenMode(window, &sdl_mode);
            if (failure)
            {
                KENGINE_ERROR("Can't change window fullscreen mode. Error: {}",
                              SDL_GetError());
            }

            fullscreen_mode.w            = sdl_mode.w;
            fullscreen_mode.h            = sdl_mode.h;
            fullscreen_mode.refresh_rate = sdl_mode.refresh_rate;
        }

        if (fullscreen)
        {
            size = { fullscreen_mode.w, fullscreen_mode.h };
            if (context)
            {
                graphics::update_viewport();
            }
        }
    }

    void set_fullscreen_mode(display::mode f_mode)
    {
        fullscreen_mode = f_mode;
        update_window_fullscreen_mode();
    }

    bool initialize(std::string_view name)
    {
        using namespace Kengine;
        bool result = true;

        configuration.load();

        configuration.override_settings_with(override_configuration);

        display::initialize();

        if (!window)
        {
            fullscreen_mode = display::get_max_mode();
            size.x = configuration.get_setting("options", "width", 640);
            size.y = configuration.get_setting("options", "height", 480);
            fullscreen =
                configuration.get_setting("options", "fullscreen", true);
            maximized =
                configuration.get_setting("options", "maximized", false);
            fullscreen_mode.w = configuration.get_setting(
                "fullscreen_options", "width", fullscreen_mode.w);
            fullscreen_mode.h = configuration.get_setting(
                "fullscreen_options", "height", fullscreen_mode.h);
            fullscreen_mode.refresh_rate =
                configuration.get_setting("fullscreen_options",
                                          "refresh_rate",
                                          fullscreen_mode.refresh_rate);

            Uint32 wnd_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                               SDL_WINDOW_HIGH_PIXEL_DENSITY;

            if (fullscreen)
                wnd_flags |= SDL_WINDOW_FULLSCREEN;

            if (maximized)
                wnd_flags |= SDL_WINDOW_MAXIMIZED;

            window = SDL_CreateWindow(name.data(), size.x, size.y, wnd_flags);
            update_window_fullscreen_mode();

            if (window == nullptr)
            {
                KENGINE_FATAL("Error to create window. Error: {}",
                              SDL_GetError());
                return false;
            }
        }

        if (!context)
        {
            // Could not create GL context: The operation completed
            // successfully.
            // TODO: What is happening?
            //         gl_minor_version = 3;
            //         gl_major_version = 2;
            //         SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
            //         SDL_GL_CONTEXT_DEBUG_FLAG);

            if (gl_debug)
            {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                                    SDL_GL_CONTEXT_DEBUG_FLAG);
            }

            KENGINE_INFO("GL required version: {}.{}",
                         gl_major_version,
                         gl_minor_version);
            KENGINE_INFO("GL required profile: {}",
                         gl_profile == SDL_GL_CONTEXT_PROFILE_ES ? "ES"
                                                                 : "Core");

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile);

            context = SDL_GL_CreateContext(window);
            if (context == nullptr)
            {
                KENGINE_FATAL("Failed to create GL context. Error: {}",
                              SDL_GetError());
                return false;
            }

            // Check GL loaded version
            {
                int gl_major_version_returned = gl_major_version;
                int gl_minor_version_returned = gl_minor_version;
                int gl_profile_returned       = gl_profile;

                if (!SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                         &gl_major_version_returned) ||
                    !SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                         &gl_minor_version_returned) ||
                    !SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                                         &gl_profile_returned))
                {
                    KENGINE_FATAL("Failed to get GL versions. Error: {}",
                                  SDL_GetError());
                    return false;
                }

                KENGINE_INFO("Open GL version: {}.{}",
                             gl_major_version_returned,
                             gl_minor_version_returned);
                KENGINE_INFO("Open GL profile: {}",
                             gl_profile_returned == SDL_GL_CONTEXT_PROFILE_ES
                                 ? "ES"
                                 : "Core");

                if (gl_major_version_returned < gl_major_version ||
                    gl_minor_version_returned < gl_minor_version ||
                    gl_profile_returned != gl_profile)
                {
                    KENGINE_FATAL("Open GL context version is low."
                                  " Minimum: {}.{}",
                                  gl_major_version,
                                  gl_minor_version);
                    return false;
                }
            }

            if (!opengl::initialize())
            {
                KENGINE_FATAL("Failed to initialize opengl.");
                return false;
            }

#ifdef KENGINE_GL_GLAD
            glad_set_post_callback(pre_call_callback_gl);
#endif

            if (gl_debug)
                if (opengl_debug::initialize(
                        gl_major_version, gl_minor_version, gl_profile))
                    KENGINE_INFO("GL debug enabled");

            if (!graphics::initialize())
            {
                KENGINE_FATAL("Failed to initialize render manager.");
                return false;
            }
        }

        update_sizes();
        return true;
    }

    void shutdown()
    {
        if (context)
        {
            graphics::shutdown();
            SDL_GL_DestroyContext(context);
            context = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

        configuration.set_setting("options", "width", size.x);
        configuration.set_setting("options", "height", size.y);
        configuration.set_setting("options", "fullscreen", fullscreen);
        configuration.set_setting("options", "maximized", maximized);
        configuration.set_setting(
            "fullscreen_options", "width", fullscreen_mode.w);
        configuration.set_setting(
            "fullscreen_options", "height", fullscreen_mode.h);
        configuration.set_setting(
            "fullscreen_options", "refresh_rate", fullscreen_mode.refresh_rate);
        configuration.save();
    }

    void warp_mouse(float x, float y)
    {
        SDL_WarpMouseInWindow(window, x, y);
    }

} // namespace Kengine::window