#pragma once

#include "Kengine/game.hxx"
#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/shader.hxx"
#include "Kengine/graphics/texture.hxx"
#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "Kengine/units/vertex.hxx"

using namespace Kengine::graphics;

class demo_game : public Kengine::game
{
public:
    demo_game()
        : Kengine::game(){};

    ~demo_game() override;
    void on_start() override;
    void on_event(Kengine::event::game_event) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;

    std::string name;

    glm::mat4 projection;
    glm::mat4 view;

private:
    vertex_array vao;

    shader  sh = shader(Kengine::make_resource<Kengine::shader_res>(
        std::filesystem::path("assets/shaders/square.vs"),
        std::filesystem::path("assets/shaders/square.fs"),
        "square_program"));
    texture checker_texture =
        texture(Kengine::make_resource<Kengine::texture_resource>(
            std::filesystem::path("assets/textures/checker.png"),
            "checker_texture"));
    camera main_camera;
};