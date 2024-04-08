#pragma once

#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/resources/material-resource.hxx"
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
} // namespace Kengine::graphics