#pragma once

#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/resources/material-resource.hxx"
#include "Kengine/units/vector.hxx"


#include <memory>

namespace Kengine::graphics
{
    void set_clear_color(const vec4& col); // set clear color

    void push_framebuffer(framebuffer& frame);
    void pop_framebuffer();

    void push_camera(std::shared_ptr<camera> cam);
    void pop_camera();

    void bind_material(const res_ptr<material_resource>& material);
} // namespace Kengine::graphics