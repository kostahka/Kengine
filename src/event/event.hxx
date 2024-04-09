#pragma once

#include "Kengine/engine.hxx"
#include "Kengine/event/event.hxx"

namespace Kengine::event
{
    bool poll_events(game* game);
    void push_gui_event(const gui_event& g_ev);
} // namespace Kengine::event
