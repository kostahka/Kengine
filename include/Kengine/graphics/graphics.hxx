#pragma once

#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/material-resource.hxx"
#include "Kengine/resources/shader-resource.hxx"
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

    E_DECLSPEC void  bind_shader(const res_ptr<shader_res>& shader);
    E_DECLSPEC void  bind_font_texture(const res_ptr<font_resource>& font,
                                       uint32_t block_index);
    E_DECLSPEC ivec2 get_current_viewport();

} // namespace Kengine::graphics