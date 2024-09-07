#pragma once

#include "Kengine/components/component.hxx"

struct player_controller_component : public Kengine::component
{
    static constexpr const char* name = "player_controller_component";
    static constexpr float       default_move_speed = 1.0f;

    player_controller_component();

    std::size_t serialize(std::ostream& os) const override;
    std::size_t deserialize(std::istream& is) override;
    std::size_t serialize_size() const override;

    bool imgui_editable_render() override;

    float         move_speed = default_move_speed;
    Kengine::vec2 move_dir{ 0, 0 };
};