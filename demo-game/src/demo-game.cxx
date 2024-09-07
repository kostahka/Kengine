#include "demo-game.hxx"

#include "player-controller-component.hxx"
#include "player-controller-system.hxx"

#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/graphics/uniformbuffer.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/main.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/scene/scene.hxx"
#include "Kengine/window/window.hxx"

#include <stdlib.h>

using namespace Kengine::graphics;

demo_game::demo_game()
    : Kengine::game()
{
    Kengine::system_container::register_system<player_controller_system>(
        player_controller_system::name);

    Kengine::component_container::register_component<
        player_controller_component>(player_controller_component::name);
}

void demo_game::on_start()
{
    set_current_scene(std::string_view("demo-main"));
    get_current_scene().instansiate(Kengine::string_id("player-ship"));
}

void demo_game::on_event(const Kengine::event::game_event& e)
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

extern "C" int main(int argc, char* argv[])
{
    Kengine::scene_manager::set_assets_base_folder("");
    if (Kengine::run(&create_game, "Kengine Demo"))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}