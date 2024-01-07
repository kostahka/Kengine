#pragma once

#include "Kengine/game-objects/component.hxx"

namespace Kengine
{
    class physics_component : public component
    {
    public:
        physics_component();
        ~physics_component();

        void on_start() override;
        void on_event(event::game_event) override;
        void on_update(int delta_ms) override;
        void on_render(int delta_ms) override;

    protected:
    private:
    };
} // namespace Kengine