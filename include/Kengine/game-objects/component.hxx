#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/game-objects/component-types.hxx"

namespace Kengine
{
    class game_object;

    class component
    {
    public:
        component(component_type type, game_object* object);
        ~component();

        virtual void on_start()                  = 0;
        virtual void on_event(event::game_event) = 0;
        virtual void on_update(int delta_ms)     = 0;
        virtual void on_render(int delta_ms)     = 0;

        inline component_type get_type() const { return type; };

    protected:
        game_object* component_object;

    private:
        component_type type;
    };
} // namespace Kengine