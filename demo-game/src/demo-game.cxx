#include "demo-game.hxx"

#include "Kengine/engine.hxx"
#include "Kengine/graphics/render-manager.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/main.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/window/window.hxx"

#include <stdlib.h>

using namespace Kengine::graphics;

void demo_game::on_start()
{
    std::vector<Kengine::vertex_text2d_color> vertices{
        {{ -0.5, -0.5, 0 },  { 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }},
        { { -0.5, 0.5, 0 },  { 1.0, 0.0 }, { 0.0, 1.0, 0.0, 1.0 }},
        { { 0.5, 0.5, 0 },   { 1.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }},

        { { 0.5, 0.5, 0 },   { 1.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }},
        { { 0.5, -0.5, 0 },  { 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }},
        { { -0.5, -0.5, 0 }, { 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }}
    };

    vbo = std::make_shared<vertex_buffer<Kengine::vertex_text2d_color>>();
    vbo->bind();
    vbo->allocate_vertices(vertices.data(), vertices.size(), false);
    vbo->add_attribute_pointer(
        { g_float, 3, 0, sizeof(Kengine::vertex_text2d_color) });
    vbo->add_attribute_pointer({ g_float,
                                 4,
                                 offsetof(Kengine::vertex_text2d_color, col),
                                 sizeof(Kengine::vertex_text2d_color) });
    vbo->add_attribute_pointer({ g_float,
                                 2,
                                 offsetof(Kengine::vertex_text2d_color, text),
                                 sizeof(Kengine::vertex_text2d_color) });

    vao = std::make_shared<vertex_array>();
    vao->bind();
    vao->add_vertex_buffer(vbo);

    sh = std::make_shared<shader>(Kengine::make_resource<Kengine::shader_res>(
        std::filesystem::path("assets/shaders/square.vs"),
        std::filesystem::path("assets/shaders/square.fs"),
        "square_program"));

    sh->save_uniform_location("time");

    checker_texture = std::make_shared<texture>(
        Kengine::make_resource<Kengine::texture_resource>(
            std::filesystem::path("assets/textures/checker.png"),
            "checker_texture"));
    checker_texture->bind();

    sh->save_uniform_location("checker");
    sh->use();

    Kengine::graphics::render_manager::set_clear_color(
        { 0.2f, 0.3f, 0.4f, 1.0f });
}

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

void demo_game::on_update(int delta_ms)
{
    sh->uniform<int>("time", Kengine::get_time_ms());
}

void demo_game::on_render(int delta_ms)
{
    vao->draw(draw_mode::triangles, 6);
}

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