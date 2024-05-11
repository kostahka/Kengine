#pragma once

#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/resources/material-resource.hxx"
#include "Kengine/resources/shader-resource.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/units/vector.hxx"

#include <memory>

namespace Kengine::graphics
{
    E_DECLSPEC void set_clear_color(const vec4& col); // set clear color

    E_DECLSPEC void push_framebuffer(framebuffer& frame);
    E_DECLSPEC void pop_framebuffer();

    E_DECLSPEC void push_camera(std::shared_ptr<camera> cam);
    E_DECLSPEC void pop_camera();

    E_DECLSPEC void bind_material(const res_ptr<material_resource>& material);
    E_DECLSPEC void bind_material(
        const res_ptr<sprite_material_resource>& material);
    E_DECLSPEC void bind_material(
        const res_ptr<gui_material_resource>& material);

    E_DECLSPEC void  bind_shader(const res_ptr<shader_res>& shader);
    E_DECLSPEC void  bind_font_texture(const res_ptr<font_resource>& font,
                                       uint32_t block_index);
    E_DECLSPEC ivec2 get_current_viewport();

    E_DECLSPEC void update_matrices(const mat4x4& projection,
                                    const mat4x4& view);
    E_DECLSPEC void update_projection(const mat4x4& projection);
    E_DECLSPEC void update_view(const mat4x4& view);
} // namespace Kengine::graphics