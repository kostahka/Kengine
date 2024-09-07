#include "player-controller-component.hxx"

#include "imgui.h"

player_controller_component::player_controller_component()
    : Kengine::component(name)
{
}

std::size_t player_controller_component::serialize(std::ostream& os) const
{
    std::size_t size = 0;

    size += Kengine::serialization::write(os, move_speed);

    return size;
}

std::size_t player_controller_component::deserialize(std::istream& is)
{
    std::size_t size = 0;

    size += Kengine::serialization::read(is, move_speed);

    return size;
}

std::size_t player_controller_component::serialize_size() const
{
    std::size_t size = 0;

    size += Kengine::serialization::size(move_speed);

    return size;
}

bool player_controller_component::imgui_editable_render()
{
    bool edited = false;
    ImGui::PushID(this);
    edited = ImGui::DragFloat("Move speed", &move_speed);
    ImGui::PopID();
    return edited;
}