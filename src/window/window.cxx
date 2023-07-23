#include "window.hxx"
#include "Kengine/window/window.hxx"

#include "SDL3/SDL_render.h"

#include "../opengl/opengl-debug.hxx"

#include <iostream>

#ifdef __ANDROID__
 #define SDL_WINDOW_RESIZABLE 0
#endif

namespace Kengine::window
{
    ivec2 size{ 0, 0 };
    ivec2 size_in_pixels{ 0, 0 };

    SDL_Window   *window  = nullptr;
    SDL_GLContext context = nullptr;

    bool gl_debug = false;

    int gl_major_version = 3;
    int gl_minor_version = 0;
    int gl_profile       = SDL_GL_CONTEXT_PROFILE_ES;

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
            glViewport(0, 0, size_in_pixels.x, size_in_pixels.y);
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

    void set_size(ivec2 s)
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

    bool initialize(std::string_view name, ivec2 size)
    {
        bool result = true;

        if (!window)
        {
            window = SDL_CreateWindow(name.data(),
                                      size.x,
                                      size.y,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

            if (window == nullptr)
            {
                std::cerr << "Error to create window. Error: " << SDL_GetError()
                          << std::endl;
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

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_version);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, gl_profile);

            context = SDL_GL_CreateContext(window);
            if (context == nullptr)
            {
                std::cerr << "Failed to create GL context. Error: "
                          << SDL_GetError() << std::endl;
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
                    std::cerr << "Failed to get GL versions. Error: "
                              << SDL_GetError() << std::endl;
                    return false;
                }

                if (gl_major_version_returned < gl_major_version ||
                    gl_minor_version_returned < gl_minor_version ||
                    gl_profile_returned != gl_profile)
                {
                    std::cerr << "Open GL context version is low. Minimum "
                                 "required: 3.0"
                              << std::endl;
                    return false;
                }
            }

            std::clog << "Open GL version: " << gl_major_version << "."
                      << gl_minor_version << std::endl;

            Kengine::opengl::initialize();

            if (gl_debug)
                Kengine::opengl_debug::initialize(
                    gl_major_version, gl_minor_version, gl_profile);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        update_sizes();
        return true;
    }

    void begin_render()
    {
        // TODO
    }

    void end_render()
    {
        // TODO
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

    SDL_Window *get_sdl_window()
    {
        return window;
    }

    SDL_GLContext get_context()
    {
        return context;
    }
} // namespace Kengine::window