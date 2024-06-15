#include "Kengine/event/custom-event.hxx"

#include <SDL3/SDL_events.h>

namespace Kengine
{
    void push_user_event(string_id event_id, void* data1, void* data2)
    {
        SDL_Event     event;
        SDL_UserEvent user_event;

        user_event.code  = static_cast<int32_t>(event_id.get_id());
        user_event.type  = SDL_EVENT_USER;
        user_event.data1 = data1;
        user_event.data2 = data2;

        event.type = SDL_EVENT_USER;
        event.user = user_event;

        SDL_PushEvent(&event);
    };
}; // namespace Kengine
