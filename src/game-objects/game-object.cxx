#include "Kengine/game-objects/game-object.hxx"

namespace Kengine
{
    game_object::game_object() {}

    game_object::~game_object() {}

    void game_object::set_position(const vec3& new_position) {}

    void game_object::set_rotation(const vec3& new_rotation) {}

    void game_object::set_scale(const vec3& new_scale) {}

    void game_object::add_component(std::unique_ptr<component> component)
    {
        components[component->get_type()].swap(component);
    }

    component* game_object::get_component(component_type type)
    {
        if (components.contains(type))
        {
            return components[type].get();
        }

        return nullptr;
    }

    void game_object::on_start()
    {
        for (auto& component_info : components)
        {
            component_info.second->on_start();
        }
    }

    void game_object::on_event(event::game_event g_event)
    {
        for (auto& component_info : components)
        {
            component_info.second->on_event(g_event);
        }
    }

    void game_object::on_update(int delta_ms)
    {
        for (auto& component_info : components)
        {
            component_info.second->on_update(delta_ms);
        }
    }

    void game_object::on_render(int delta_ms)
    {
        for (auto& component_info : components)
        {
            component_info.second->on_render(delta_ms);
        }
    }
} // namespace Kengine