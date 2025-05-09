#include "Kengine/io/input.hxx"
#include "Kengine/engine.hxx"
#include "SDL3/SDL_clipboard.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_stdinc.h"
#include "imgui.h"

#include <array>

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_touch.h>
#include <string>

namespace Kengine::input
{
    namespace mouse
    {

        std::array<bool, buttons_count> b_pressed{
            false, false, false, false, false
        };

        float x;
        float y;

        void update()
        {
            const Uint32 state = SDL_GetMouseState(&x, &y);
            for (int i = 0; i < buttons_count; i++)
            {
                b_pressed[i] = state & SDL_BUTTON_MASK(i + 1);
            }
        }

        bool button_pressed(button b)
        {
            return b_pressed[static_cast<int>(b) - 1];
        }

        void set_button_pressed(button b, bool pressed)
        {
            b_pressed[static_cast<int>(b) - 1] = pressed;
        };

    } // namespace mouse

    namespace keyboard
    {
        std::array<bool, keys_count> k_pressed{};

        void update()
        {
            const bool* state = SDL_GetKeyboardState(nullptr);
            for (int i = static_cast<int>(key::key_first); i < keys_count; i++)
            {
                k_pressed[i] = state[i];
            }
        };

        void set_key_pressed(key k, bool pressed)
        {
            k_pressed[static_cast<int>(k)] = pressed;
        };

        bool key_pressed(key k)
        {
            return k_pressed[static_cast<int>(k)];
        };
    } // namespace keyboard

    namespace touch
    {
        // Not working, because SDL_GetTouchFinger await for finger index in
        // array implemented in SDL3 and not fingerId. SDL3 has no declaration
        // to get finger index
        //
        // bool get_touch_pos(int64_t touch_id, int64_t finger_id, float* x,
        // float* y)
        //{
        //    SDL_Finger* finger = SDL_GetTouchFinger(touch_id, finger_id);
        //    if (finger)
        //    {
        //        *x = finger->x *
        //        engine::instance()->e_game->configuration.screen_width; *y =
        //            finger->y *
        //            engine::instance()->e_game->configuration.screen_height;
        //        return true;
        //    }
        //    return false;
        //}
    } // namespace touch

    namespace clipboard
    {
        bool set_text(const char* text)
        {
            if(SDL_SetClipboardText(text))
            {
                return true;
            }

            KENGINE_ERROR("Failed to set clipboard text. Error: {}", SDL_GetError());
            return false;
        }

        std::string get_text()
        {
            char* text = SDL_GetClipboardText();
            if(!text)
            {
                KENGINE_ERROR("Failed to get clipboard text. Error: {}", SDL_GetError());
                return {};
            }
            
            std::string result(text);
            SDL_free(text);
            return result;
        }
    } // namespace clipboard
} // namespace Kengine::input