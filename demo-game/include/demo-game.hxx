#pragma once

#include "Kengine/game.hxx"
#include "Kengine/graphics/shader.hxx"
#include "Kengine/graphics/texture.hxx"
#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/units/vertex.hxx"

using namespace Kengine::graphics;

class demo_game : public Kengine::game
{
public:
    demo_game()
        : Kengine::game("Demo kengine"){};

    ~demo_game() override;
    void on_start() override;
    void on_event(Kengine::event::game_event) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;
    void on_imgui_render() override;

    std::string name;

    glm::mat4 projection;
    glm::mat4 view;

private:
    std::shared_ptr<vertex_buffer<Kengine::vertex_text2d_color>> vbo{ nullptr };
    std::shared_ptr<vertex_array>                                vao{ nullptr };
    std::shared_ptr<shader>                                      sh{ nullptr };
    std::shared_ptr<texture> checker_texture{ nullptr };
};