#include "Kengine/resources/font-material-resource.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/imgui/imgui-edit.hxx"

#include "imgui.h"

namespace Kengine
{
    font_material_resource::font_material_resource(std::string_view name)
        : material_resource(graphics::text_shader, name)
    {
        r_type = resource_type::font_material;
    }

    font_material_resource::font_material_resource(
        std::string_view name, const res_ptr<font_resource>& font)
        : material_resource(graphics::text_shader, name)
        , font(font)
    {
        r_type = resource_type::font_material;
    }

    font_material_resource::~font_material_resource() {}

    std::size_t font_material_resource::serialize(std::ostream& os) const
    {
        std::size_t size = material_resource::serialize(os);
        return size;
    }

    std::size_t font_material_resource::deserialize(std::istream& is)
    {
        std::size_t size = material_resource::deserialize(is);
        font             = static_resource_cast<font_resource>(textures[0]);
        return size;
    }

    std::size_t font_material_resource::serialize_size() const
    {
        std::size_t size = material_resource::serialize_size();
        return size;
    }

    bool font_material_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        if (imgui::edit_resource("Font", &font))
        {
            textures[0] = static_resource_cast<texture_resource>(font);
        }

        ImGui::PopID();
#endif
        return edited;
    }
} // namespace Kengine