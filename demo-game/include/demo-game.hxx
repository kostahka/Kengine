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

private:
};