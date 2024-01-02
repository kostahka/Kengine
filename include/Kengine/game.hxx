#pragma once

#include <string>

#include "Kengine/event/event.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/window/window.hxx"

#include "glm/glm.hpp"

namespace Kengine
{
    class game
    {
    public:
        game() = default;

        virtual ~game()                          = default;
        virtual void on_start()                  = 0;
        virtual void on_event(event::game_event) = 0;
        virtual void on_update(int delta_ms)     = 0;
        virtual void on_render(int delta_ms)     = 0;

        virtual on_imgui_render* get_imgui_render() { return nullptr; };

        glm::mat4 projection{};
        glm::mat4 view{};
    };
} // namespace Kengine