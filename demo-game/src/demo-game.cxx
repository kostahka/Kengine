#include "demo-game.hxx"

#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/graphics/uniformbuffer.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/main.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/window/window.hxx"

#include <stdlib.h>

using namespace Kengine::graphics;

void demo_game::on_start() {}

void demo_game::on_event(Kengine::event::game_event e)
{
    if (e.g_type == Kengine::event::type::keyboard_event)
    {
        if (e.keyboard.key == Kengine::input::keyboard::key::key_escape &&
            e.keyboard.pressed)
        {
            Kengine::quit();
        }
    }
}

void demo_game::on_update(int delta_ms) {}

void demo_game::on_render(int delta_ms) {}

demo_game::~demo_game() {}

Kengine::game* create_game()
{
    return new demo_game();
}

int main()
{
    if (Kengine::run(&create_game, "Kengine Demo"))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}