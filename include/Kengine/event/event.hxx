#pragma once

#include "Kengine/event/gui-event.hxx"
#include "Kengine/io/input.hxx"
#include "Kengine/units/vector.hxx"
#include <cstdint>

namespace Kengine::event
{
    enum class type
    {
        keyboard_event,
        mouse_button_event,
        mouse_motion_event,
        touch_event,
        touch_move_event,
        window_resize,
        window_maximazed,
        window_hide_show,
        gui_event,
        custom_event,

        quit,

        unknown
    };

    struct mouse_button_event
    {
        input::mouse::button button;
        uint8_t              clicks;
        bool                 pressed;
        float                x;
        float                y;
    };

    struct mouse_motion_event
    {
        float x;
        float y;
        float x_motion;
        float y_motion;
    };

    struct keyboard_event
    {
        input::keyboard::key key;
        bool                 pressed;
    };

    struct touch_event
    {
        int64_t touch_id;
        int64_t finger_id;
        float   x;
        float   y;
        bool    pressed;
    };

    struct window_event
    {
        union
        {
            bool  maximized;
            bool  hidden;
            ivec2 new_size;
        };
    };

    struct custom_event
    {
        string_id event_id{};
        void*     data1;
        void*     data2;
    };

    struct game_event
    {
        type g_type;

        union
        {
            mouse_button_event mouse;
            mouse_motion_event motion;
            keyboard_event     keyboard;
            touch_event        touch;
            window_event       window;
            gui_event          gui;
            custom_event       custom;
        };
    };

} // namespace Kengine::event
