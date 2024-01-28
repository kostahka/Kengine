#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/units/rect.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    struct sprite_component : public component
    {
        sprite_component();
        sprite_component(entt::entity,
                         const res_ptr<sprite_material_resource>& material,
                         rect uv     = { 0, 0, 1, 1 },
                         vec2 origin = { 0, 0 });
        ~sprite_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        inline const entt::entity get_entity() const { return ent; }

        inline const res_ptr<sprite_material_resource>& get_material() const
        {
            return material;
        }

        vec2 origin = { 0, 0 };
        rect uv{ 0, 0, 1, 1 };

    private:
        entt::entity                      ent      = entt::null;
        res_ptr<sprite_material_resource> material = nullptr;
    };

} // namespace Kengine