#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/units/vector.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    class scene;

    struct transform
    {
        vec2  position = { 0, 0 };
        float angle    = 0;
        vec3  scale    = { 1, 1, 1 };
    };

    struct transform_component : public component
    {
        static constexpr auto name = "transform_component";
        static component_info info;

        transform_component();
        transform_component(scene* sc, entt::entity cur_entity);

        transform_component(transform_component& other) = delete;
        transform_component(transform_component& other,
                            scene*               sc,
                            entt::entity         cur_entity,
                            entt::entity         new_parent);
        transform_component(transform_component&& other);

        transform_component& operator=(transform_component& other);
        transform_component& operator=(transform_component&& other);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        transform get_world_transform();

        transform transform;

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