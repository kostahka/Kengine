#pragma once

#include "Kengine/resources/material-resource.hxx"

namespace Kengine
{
    class E_DECLSPEC sprite_material_resource : public material_resource
    {
    public:
        sprite_material_resource(std::string_view name);
        sprite_material_resource(
            const res_ptr<fragment_shader_res>& fragment_shader,
            std::string_view                    shader_name,
            std::string_view                    name);
        ~sprite_material_resource();

        bool imgui_editable_render() override;

    private:
    };
} // namespace Kengine