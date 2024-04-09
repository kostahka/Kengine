#include "Kengine/event/event.hxx"
#include "event.hxx"

#include "../engine.hxx"
#include "../window/window.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/game.hxx"
#include "Kengine/helpers/spin-lock.hxx"
#include "Kengine/window/window.hxx"
#include "imgui_impl_sdl3.h"

#include <SDL_events.h>
#include <SDL_mouse.h>

#include <queue>

namespace Kengine::event
{
    static spin_lock             gui_events_lock{};
    static std::queue<gui_event> gui_events{};

    bool poll_events(game* game)
    {
        bool      no_quit = true;
        SDL_Event sdl_event;

        while (SDL_PollEvent(&sdl_event))
        {
            game_event event{ event::type::unknown };

            ivec2 window_size;

            switch (sdl_event.type)
            {
                case SDL_EVENT_WINDOW_HIDDEN:
                case SDL_EVENT_WINDOW_SHOWN:
                    Kengine::window::update_flags();
                    event.g_type        = type::window_hide_show;
                    event.window.hidden = Kengine::window::is_hidden();
                    break;
                case SDL_EVENT_WINDOW_MAXIMIZED:
                case SDL_EVENT_WINDOW_MINIMIZED:
                case SDL_EVENT_WINDOW_RESTORED:
                    Kengine::window::update_flags();
                    event.g_type           = type::window_maximazed;
                    event.window.maximized = Kengine::window::is_maximized();
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    Kengine::window::update_flags();
                    Kengine::window::update_sizes();
                    event.g_type          = type::window_resize;
                    event.window.new_size = Kengine::window::get_size();
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
                    input::mouse::y = sdl_event.motion.y;
                    if (sdl_event.motion.which != SDL_TOUCH_MOUSEID)
                    {
                        event.g_type          = type::mouse_motion_event;
                        event.motion.x        = sdl_event.motion.x;
                        event.motion.y        = sdl_event.motion.y;
                        event.motion.x_motion = sdl_event.motion.xrel;
                        event.motion.y_motion = sdl_event.motion.yrel;
                    }
                    break;
                case SDL_EVENT_FINGER_MOTION:
                    window_size           = window::get_size_in_pixels();
                    event.g_type          = type::touch_move_event;
                    event.touch.touch_id  = sdl_event.tfinger.touchID;
                    event.touch.finger_id = sdl_event.tfinger.fingerID;
                    event.touch.x         = sdl_event.tfinger.x * window_size.x;
                    event.touch.y         = sdl_event.tfinger.y * window_size.y;
                    event.touch.pressed   = true;
                    break;
                case SDL_EVENT_FINGER_UP:
                case SDL_EVENT_FINGER_DOWN:
                    window_size           = window::get_size_in_pixels();
                    event.g_type          = type::touch_event;
                    event.touch.touch_id  = sdl_event.tfinger.touchID;
                    event.touch.finger_id = sdl_event.tfinger.fingerID;
                    event.touch.x         = sdl_event.tfinger.x * window_size.x;
                    event.touch.y         = sdl_event.tfinger.y * window_size.y;
                    event.touch.pressed =
                        sdl_event.type == SDL_EVENT_FINGER_DOWN;

                    if (event.touch.pressed)
                        window::warp_mouse(sdl_event.tfinger.x * window_size.x,
                                           sdl_event.tfinger.y * window_size.y);
                    break;
                case SDL_EVENT_QUIT:
                    event.g_type = type::quit;
                    no_quit      = false;
                    break;
                case SDL_EVENT_USER:
                    event.g_type          = type::custom_event;
                    event.custom.event_id = sdl_event.user.code;
                    event.custom.data1    = sdl_event.user.data1;
                    event.custom.data2    = sdl_event.user.data2;
                    break;
                default:
                    break;
            }
#ifdef KENGINE_IMGUI
            ImGui_ImplSDL3_ProcessEvent(&sdl_event);
#endif
            if (event.g_type != type::unknown)
            {
                Kengine::e_game->on_event(event);
                Kengine::e_game->get_current_scene().on_event(event);
            }
        }

        {
            std::lock_guard lock(gui_events_lock);
            while (gui_events.size() > 0)
            {
                game_event event{ event::type::gui_event };
                event.gui = gui_events.front();
                Kengine::e_game->on_event(event);
                Kengine::e_game->get_current_scene().on_event(event);
                gui_events.pop();
            }
        }

        return no_quit;
    };

    void push_gui_event(const gui_event& g_ev)
    {
        std::lock_guard lock(gui_events_lock);
        gui_events.emplace(g_ev);
    }
} // namespace Kengine::event
