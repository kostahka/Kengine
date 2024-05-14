#pragma once

#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/material-resource.hxx"

namespace Kengine
{
    class E_DECLSPEC font_material_resource : public material_resource
    {
    public:
        font_material_resource(std::string_view name);
        font_material_resource(std::string_view              name,
                               const res_ptr<font_resource>& font);
        ~font_material_resource();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        inline res_ptr<font_resource>& get_font() { return font; }

    private:
        res_ptr<font_resource> font;
    };
} // namespace Kengine