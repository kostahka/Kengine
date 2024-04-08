#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/units/rect.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    struct E_DECLSPEC gui_component : public component
    {
        static constexpr auto name = "gui_component";

        gui_component();

        gui_component(gui_component& other);
        gui_component(gui_component&& other);

        gui_component& operator=(gui_component& other);
        gui_component& operator=(gui_component&& other);

        ~gui_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        void set_material(const res_ptr<gui_material_resource>& material);

        inline const res_ptr<gui_material_resource>& get_material() const
        {
            return material;
        }

        vec2  origin = { 0, 0 };
        rect  uv{ 0, 0, 1, 1 };
        float angle = 0;
        vec2  scale{ 1, 1 };
        int   layer = 0;

    private:
        res_ptr<gui_material_resource> material = nullptr;
    };

} // namespace Kengine