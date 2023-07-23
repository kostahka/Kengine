#pragma once

#include <string>

#include "Kengine/event/event.hxx"

#include "glm/glm.hpp"

namespace Kengine
{
    class game
    {
    public:
        game(std::string name)
            : name(name){};

        virtual ~game()                          = default;
        virtual void on_start()                  = 0;
        virtual void on_event(event::game_event) = 0;
        virtual void on_update(int delta_ms)     = 0;
        virtual void on_render(int delta_ms)     = 0;
        virtual void on_imgui_render()           = 0;

        std::string name;

        glm::mat4 projection;
        glm::mat4 view;
    };
} // namespace Kengine