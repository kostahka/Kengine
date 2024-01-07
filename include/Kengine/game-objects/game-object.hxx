#pragma once

#include "Kengine/game-objects/component.hxx"
#include "Kengine/units/vector.hxx"

#include <memory>
#include <unordered_map>

namespace Kengine
{
    class game_object
    {
    public:
        game_object();
        ~game_object();

        virtual void set_position(const vec3& new_position);
        virtual void set_rotation(const vec3& new_rotation);
        virtual void set_scale(const vec3& new_scale);

        void       add_component(std::unique_ptr<component> component);
        component* get_component(component_type);

        virtual void on_start();
        virtual void on_event(event::game_event);
        virtual void on_update(int delta_ms);
        virtual void on_render(int delta_ms);

        inline const vec3& get_position() const { return position; };

        inline const vec3& get_rotation() const { return rotation; };

        inline const vec3& get_scale() const { return scale; };

    private:
        vec3 position;
        vec3 rotation;
        vec3 scale;

        std::unordered_map<component_type, std::unique_ptr<component>>
            components;
    };
} // namespace Kengine