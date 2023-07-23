#include "Kengine/event/event.hxx"

#include "../engine.hxx"
#include "../window/window.hxx"
#include "Kengine/engine.hxx"
#include "event-engine.hxx"
#include "handle-user-event.hxx"
#include "imgui_impl_sdl3.h"

#include <SDL_events.h>
#include <SDL_mouse.h>

#include <iostream>

namespace Kengine::event
{
    bool poll_events(game *game, SDL_Window *window)
    {
        bool      no_quit = true;
        SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event))
        {
            game_event event{ event::type::unknown };

            int screen_width;
            int screen_height;

            switch (sdl_event.type)
            {
                case SDL_EVENT_WINDOW_RESIZED:
                    Kengine::window::update_sizes();
                    event.g_type = type::window_resize;
                    break;
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_KEY_UP:
                    event.g_type       = type::keyboard_event;
                    event.keyboard.key = static_cast<input::keyboard::key>(
                        sdl_event.key.keysym.scancode);
                    event.keyboard.pressed =
                        sdl_event.type == SDL_EVENT_KEY_DOWN;

                    input::keyboard::set_key_pressed(event.keyboard.key,
                                                     event.keyboard.pressed);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (ImGui::GetIO().WantCaptureMouse)
                    {
                        event.g_type = type::unknown;
                        break;
                    }
                    event.g_type       = type::mouse_button_event;
                    event.mouse.button = static_cast<input::mouse::button>(
                        SDL_BUTTON(sdl_event.button.button));
                    event.mouse.pressed =
                        sdl_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
                    event.mouse.clicks = sdl_event.button.clicks;
                    event.mouse.x      = sdl_event.button.x;
                    event.mouse.y      = sdl_event.button.y;

                    input::mouse::set_button_pressed(event.mouse.button,
                                                     event.mouse.pressed);
                    input::mouse::x = event.mouse.x;
                    input::mouse::y = event.mouse.y;
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    input::mouse::x = sdl_event.motion.x;
                    input::mouse::y = sdl_event.button.y;
                    break;
                case SDL_EVENT_FINGER_MOTION:
                    SDL_GetWindowSize(window, &screen_width, &screen_height);
                    event.g_type          = type::touch_move_event;
                    event.touch.touch_id  = sdl_event.tfinger.touchId;
                    event.touch.finger_id = sdl_event.tfinger.fingerId;
                    event.touch.x         = sdl_event.tfinger.x * screen_width;
                    event.touch.y         = sdl_event.tfinger.y * screen_height;
                    event.touch.pressed   = true;
                    break;
                case SDL_EVENT_FINGER_UP:
                case SDL_EVENT_FINGER_DOWN:
                    SDL_GetWindowSize(window, &screen_width, &screen_height);
                    event.g_type          = type::touch_event;
                    event.touch.touch_id  = sdl_event.tfinger.touchId;
                    event.touch.finger_id = sdl_event.tfinger.fingerId;
                    event.touch.x         = sdl_event.tfinger.x * screen_width;
                    event.touch.y         = sdl_event.tfinger.y * screen_height;
                    event.touch.pressed =
                        sdl_event.type == SDL_EVENT_FINGER_DOWN;

                    std::cout << "Finger #" << event.touch.finger_id << " "
                              << (event.touch.pressed ? "pressed" : "unpressed")
                              << std::endl;
                    if (sdl_event.type == SDL_EVENT_FINGER_DOWN)
                        SDL_WarpMouseInWindow(
                            window,
                            sdl_event.tfinger.x * screen_width,
                            sdl_event.tfinger.y * screen_height);
                    break;
                case SDL_EVENT_QUIT:
                    event.g_type = type::quit;
                    no_quit      = false;
                    break;
                case SDL_EVENT_USER:
                    event.g_type = type::unknown;
                    handle_user_event(sdl_event.user);
                default:
                    event.g_type = type::unknown;
            }

            ImGui_ImplSDL3_ProcessEvent(&sdl_event);

            if (event.g_type != type::unknown)
                Kengine::e_game->on_event(event);
        }

        // SDL_PumpEvents();
        // input::keyboard::update();
        // input::mouse::update();

        return no_quit;
    };
} // namespace Kengine::event
