#pragma once

#include "Kengine/io/input.hxx"
#include "Kengine/string/string-id.hxx"

namespace Kengine::event
{
    enum gui_event_type
    {
        mouse_event,
        hover_event
    };

    struct gui_mouse_event
    {
        input::mouse::button button;
        uint8_t              clicks;
        bool                 pressed;
    };

    struct gui_hover_event
    {
        bool hovered = false;
    };

    struct gui_event
    {
        string_id gui_id{};

        union
        {
            gui_mouse_event mouse;
            gui_hover_event hover;
        };
    };
} // namespace Kengine::event