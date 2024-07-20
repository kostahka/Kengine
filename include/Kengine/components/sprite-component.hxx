#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/types/rect.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    struct E_DECLSPEC sprite_component : public component
    {
        static constexpr auto name = "sprite_component";

        sprite_component();
        sprite_component(const res_ptr<sprite_material_resource>& material,
                         rect uv     = { 0, 0, 1, 1 },
                         vec2 origin = { 0, 0 });

        sprite_component(sprite_component& other);
        sprite_component(sprite_component&& other);

        sprite_component& operator=(sprite_component& other);
        sprite_component& operator=(sprite_component&& other);

        ~sprite_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        void set_material(const res_ptr<sprite_material_resource>& material);

        inline const res_ptr<sprite_material_resource>& get_material() const
        {
            return material;
        }

        inline res_ptr<sprite_material_resource>& get_material()
        {
            return material;
        }

        vec2 origin = { 0, 0 };
        rect uv{ 0, 0, 1, 1 };
        int  layer   = 0;
        bool visible = true;

    private:
        res_ptr<sprite_material_resource> material = nullptr;
    };

} // namespace Kengine