#include "demo-game.hxx"

#include "Kengine/engine.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/window/window.hxx"

#include <stdlib.h>

using namespace Kengine::graphics;

void demo_game::on_start()
{
    std::vector<Kengine::vertex_color> vertices{
        {{ -0.5, -0.5, 0 },  { 1.0, 0.0, 0.0, 1.0 }},
        { { -0.5, 0.5, 0 },  { 0.0, 1.0, 0.0, 1.0 }},
        { { 0.5, 0.5, 0 },   { 0.0, 0.0, 1.0, 1.0 }},

        { { 0.5, 0.5, 0 },   { 0.0, 0.0, 1.0, 1.0 }},
        { { 0.5, -0.5, 0 },  { 0.0, 1.0, 0.0, 1.0 }},
        { { -0.5, -0.5, 0 }, { 1.0, 0.0, 0.0, 1.0 }}
    };

    vbo = std::make_shared<vertex_buffer<Kengine::vertex_color>>();
    vbo->bind();
    vbo->allocate_vertices(vertices.data(), vertices.size(), false);
    vbo->add_attribute_pointer(
        { g_float, 3, 0, sizeof(Kengine::vertex_color) });
    vbo->add_attribute_pointer({ g_float,
                                 4,
                                 offsetof(Kengine::vertex_color, col),
                                 sizeof(Kengine::vertex_color) });

    vao = std::make_shared<vertex_array>();
    vao->bind();
    vao->add_vertex_buffer(vbo);

    sh = std::make_shared<shader>(
        std::make_shared<Kengine::shader_res>(std::string_view(R"(
    #version 300 es
    precision mediump float;

    layout (location = 0) in vec3 v_pos;
    layout (location = 1) in vec4 v_color;

    out vec4 vs_color;
    void main()
    {
        gl_Position = vec4(v_pos, 1.0);
        vs_color = v_color;
    }
)"),
                                              std::string_view(R"(
    #version 300 es
    precision mediump float;

    in vec4 vs_color;

    uniform int time;

    out vec4 f_color;
    void main()
    {
        f_color = vec4(vs_color.r, vs_color.g * (sin(float(time) / 500.0 + 1.0) / 2.0), vs_color.ba);
    }
                             )")));
    sh->save_uniform_location("time");
    sh->use();
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
    sh->uniform1i("time", Kengine::get_time_ms());
}

void demo_game::on_render(int delta_ms)
{
    vao->draw(draw_mode::triangles, 6);
}

void demo_game::on_imgui_render() {}

demo_game::~demo_game() {}

Kengine::game* create_game()
{
    return new demo_game();
}

int main()
{
    if (Kengine::run(&create_game))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}