#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/units/transform.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    class scene;

    struct E_DECLSPEC rect_transform_component : public component
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

        rect_data transf;

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

        inline rect_transform get_last_transform() const { return last_trans; }

        inline void invalidate_transform() { is_trans_valid = false; }

        inline bool is_transform_valid() const { return is_trans_valid; }

    private:
        bool check_for_parent(entt::entity ent);

        entt::entity   parent         = entt::null;
        entt::entity   current_entity = entt::null;
        scene*         sc;
        bool           is_trans_valid = false;
        rect_transform last_trans;
    };
} // namespace Kengine