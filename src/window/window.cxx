#include "window.hxx"
#include "Kengine/window/window.hxx"

#include "SDL3/SDL_render.h"

#include "../graphics/render-manager.hxx"
#include "../opengl/opengl-debug.hxx"
#include "Kengine/log/log.hxx"

#ifdef __ANDROID__
 #define SDL_WINDOW_RESIZABLE 0
#endif

namespace Kengine::window
{
    options start_options;

    ivec2 size{ 0, 0 };
    ivec2 size_in_pixels{ 0, 0 };

    SDL_Window*   window  = nullptr;
    SDL_GLContext context = nullptr;

    SDL_DisplayID                primary_display;
    std::vector<SDL_DisplayMode> display_modes;

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
            KENGINE_GL_CHECK(
                glViewport(0, 0, size_in_pixels.x, size_in_pixels.y));
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

    void set_size(ivec2& s)
    {
        SDL_SetWindowSize(window, s.x, s.y);
        update_sizes();
    }

    void set_fullscreen(bool fullscreen)
    {
        if (window)
            SDL_SetWindowFullscreen(window, (SDL_bool)fullscreen);
    }

    void set_input_focus()
    {
        if (window)
            SDL_SetWindowInputFocus(window);
    }

    void raise()
    {
        if (window)
            SDL_RaiseWindow(window);
    }

    void set_options(options& o)
    {
        start_options = o;
    }

    bool initialize(std::string_view name)
    {
        bool result = true;

        size = start_options.size;

        gl_major_version = start_options.gl_major_version;
        gl_minor_version = start_options.gl_minor_version;
        gl_profile = start_options.gl_profile_es ? SDL_GL_CONTEXT_PROFILE_ES
                                                 : SDL_GL_CONTEXT_PROFILE_CORE;
        gl_debug   = start_options.gl_debug;

        if (!window)
        {
            primary_display = SDL_GetPrimaryDisplay();
            if (!primary_display)
            {
                KENGINE_FATAL("Failed to get primary display. Error: {}",
                              SDL_GetError());
                return false;
            }
            {
                int                     d_modes_count;
                const SDL_DisplayMode** modes = SDL_GetFullscreenDisplayModes(
                    primary_display, &d_modes_count);
                if (!modes)
                {
                    KENGINE_FATAL(
                        "Failed to get display fullscreen modes. Error: {}",
                        SDL_GetError());
                    return false;
                }
                display_modes.resize(d_modes_count);
                for (int i = 0; i < d_modes_count; i++)
                {
                    display_modes[i] = *modes[i];
                }
                SDL_free(modes);
            }

            window = SDL_CreateWindow(
                name.data(), size.x, size.y, SDL_WINDOW_OPENGL);

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
                         start_options.gl_profile_es ? "ES" : "Core");

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

                if (SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                        &gl_major_version_returned) ||
                    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                        &gl_minor_version_returned) ||
                    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
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
                             start_options.gl_profile_es ? "ES" : "Core");

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

            if (!Kengine::opengl::initialize())
            {
                KENGINE_FATAL("Failed to initialize opengl.");
                return false;
            }

            glad_set_post_callback(pre_call_callback_gl);

            if (gl_debug)
                if (Kengine::opengl_debug::initialize(
                        gl_major_version, gl_minor_version, gl_profile))
                    KENGINE_INFO("GL debug enabled");

            if (!graphics::render_manager::initialize())
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
            SDL_GL_DeleteContext(context);
            context = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }
    }

    SDL_Window* get_sdl_window()
    {
        return window;
    }

    SDL_GLContext get_context()
    {
        return context;
    }

    void warp_mouse(float x, float y)
    {
        SDL_WarpMouseInWindow(window, x, y);
    }

    void set_start_options(options& opt)
    {
        start_options = opt;
    }
} // namespace Kengine::window