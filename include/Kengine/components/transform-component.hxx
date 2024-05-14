#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/units/transform.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    class scene;

    struct E_DECLSPEC transform_component : public component
    {
        friend class archive_continuous_input;

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

        transform transf;

        inline void initialize(scene* sc, entt::entity cur_entity)
        {
            this->sc             = sc;
            this->current_entity = cur_entity;
        };

        inline void initialize(scene* sc) { this->sc = sc; };

        void set_parent(entt::entity ent);

        inline entt::entity get_parent() const { return parent; }

        inline entt::entity get_current_entity() const
        {
            return current_entity;
        }

    private:
        bool check_for_parent(entt::entity ent);

        entt::entity parent         = entt::null;
        entt::entity current_entity = entt::null;
        scene*       sc;
    };
} // namespace Kengine