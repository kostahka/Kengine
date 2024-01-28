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

static Kengine::vec2 camera_pos   = { 0, 0 };
constexpr float      camera_speed = 0.001f;

void demo_game::on_start()
{
    using namespace Kengine;
    std::vector<Kengine::vertex_text2d_color> vertices{
        {{ -0.5, -0.5, 0 },  { 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }},
        { { -0.5, 0.5, 0 },  { 1.0, 0.0 }, { 0.0, 1.0, 0.0, 1.0 }},
        { { 0.5, 0.5, 0 },   { 1.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }},

        { { 0.5, 0.5, 0 },   { 1.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }},
        { { 0.5, -0.5, 0 },  { 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }},
        { { -0.5, -0.5, 0 }, { 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }}
    };

    auto vbo = std::make_shared<vertex_buffer<Kengine::vertex_text2d_color>>();

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

    vao.bind();
    vao.add_vertex_buffer(vbo);

    sh.save_uniform_location("time");

    checker_texture.bind();

    sh.save_uniform_location("checker");
    sh.uniform_block("Matrices", 0);
    sh.use();

    Kengine::graphics::set_clear_color({ 0.2f, 0.3f, 0.4f, 1 });

    main_camera.bind();
    main_camera.set_view(
        glm::lookAt(Kengine::vec3(camera_pos.x, camera_pos.y, 0.5f),
                    Kengine::vec3(camera_pos.x, camera_pos.y, -5.f),
                    Kengine::vec3(0, 1, 0)));

    auto current_scene = Kengine::scene_manager::get_current_scene();

    auto chess_entity = current_scene->registry.create();
    current_scene->registry.emplace<Kengine::transform_component>(
        chess_entity, vec{ 0.5f, 0.5f, -0.1f });

    res_ptr<sprite_material_resource> chess_material =
        make_resource<sprite_material_resource>("chess_material");
    chess_material->set_texture(0, checker_texture_res);

    current_scene->registry.emplace<Kengine::sprite_component>(
        chess_entity,
        chess_entity,
        chess_material,
        rect{ 0.f, 0.f, 1.f, 1.f },
        vec2{ 0.5f, 0.5f });
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
    using namespace Kengine;
    if (input::keyboard::key_pressed(input::keyboard::key::key_left))
    {
        camera_pos.x -= camera_speed * delta_ms;
    }
    else if (input::keyboard::key_pressed(input::keyboard::key::key_right))
    {
        camera_pos.x += camera_speed * delta_ms;
    }
    else if (input::keyboard::key_pressed(input::keyboard::key::key_up))
    {
        camera_pos.y += camera_speed * delta_ms;
    }
    else if (input::keyboard::key_pressed(input::keyboard::key::key_down))
    {
        camera_pos.y -= camera_speed * delta_ms;
    }

    main_camera.set_view(
        glm::lookAt(Kengine::vec3(camera_pos.x, camera_pos.y, 0.5f),
                    Kengine::vec3(camera_pos.x, camera_pos.y, -5.f),
                    Kengine::vec3(0, 1, 0)));
}

void demo_game::on_render(int delta_ms)
{
    sh.use();
    sh.uniform<int>("time", Kengine::get_time_ms());
    vao.bind();
    vao.draw(draw_mode::triangles, 6);
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