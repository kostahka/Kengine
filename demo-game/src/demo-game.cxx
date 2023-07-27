#include "demo-game.hxx"

#include "Kengine/engine.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/window/window.hxx"

void demo_game::on_start() {}

void demo_game::on_event(Kengine::event::game_event e)
{
    if (e.g_type == Kengine::event::type::keyboard_event)
    {
        if (e.keyboard.key == Kengine::input::keyboard::key::key_escape &&
            e.keyboard.pressed)
        {
            Kengine::quit();
            KENGINE_FATAL("EXIT PRESSED");
        }
    }
}

void demo_game::on_update(int delta_ms) {}

void demo_game::on_render(int delta_ms) {}

void demo_game::on_imgui_render() {}

int main()
{

    demo_game* demo_g = new demo_game();
    Kengine::set_game(demo_g);

    Kengine::window::options w_options;
    w_options.gl_major_version = 4;
    w_options.gl_minor_version = 3;
    w_options.gl_profile_es    = false;
    w_options.gl_debug         = true;
    Kengine::window::set_start_options(w_options);

    Kengine::initialize();
    Kengine::start_game_loop();
    Kengine::shutdown();
    return 0;
}