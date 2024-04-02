#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/units/vector.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    class scene;

    struct rect_transform
    {
        vec2 start{ 0, 0 };
        vec2 rect{ 1, 1 };
    };

    struct rect_data
    {
        vec2 anchor_min = { 0.5, 0.5 };
        vec2 anchor_max = { 0.5, 0.5 };
        vec2 delta_min  = { 0, 0 };
        vec2 delta_max  = { 0, 0 };
    };

    struct rect_transform_component : public component
    {
        static constexpr auto name = "rect_transform_component";
        static component_info info;

        rect_transform_component();
        rect_transform_component(scene* sc, entt::entity cur_entity);

        rect_transform_component(rect_transform_component& other) = delete;
        rect_transform_component(rect_transform_component& other,
                                 scene*                    sc,
                                 entt::entity              cur_entity,
                                 entt::entity              new_parent);
        rect_transform_component(rect_transform_component&& other);

        rect_transform_component& operator=(rect_transform_component& other);
        rect_transform_component& operator=(rect_transform_component&& other);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        rect_transform get_world_transform();

        rect_data transform;

        inline void initialize(scene* sc, entt::entity cur_entity)
        {
            this->sc             = sc;
            this->current_entity = cur_entity;
        };

        inline void initialize(scene* sc) { this->sc = sc; };

        void set_parent(entt::entity ent);

        inline entt::entity get_parent() const { return parent; }

    private:
        bool check_for_parent(entt::entity ent);

        entt::entity parent         = entt::null;
        entt::entity current_entity = entt::null;
        scene*       sc;
    };
} // namespace Kengine