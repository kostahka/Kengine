#pragma once

namespace Kengine
{
    enum class user_events
    {
        file_modified = 0,
    };

    using user_event_func = void (*)(void *data);

    void push_user_event(int             user_event_code,
                         user_event_func event_func,
                         void           *data);

}; // namespace Kengine
