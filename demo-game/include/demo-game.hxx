#pragma once

#include "Kengine/game.hxx"

class demo_game : public Kengine::game
{
public:
    demo_game()
        : Kengine::game("Demo kengine"){};

    virtual ~demo_game() = default;
    void on_start() override;
    void on_event(Kengine::event::game_event) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;
    void on_imgui_render() override;

    std::string name;

    glm::mat4 projection;
    glm::mat4 view;
};