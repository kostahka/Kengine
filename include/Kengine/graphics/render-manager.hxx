#pragma once

#include "../units/vector.hxx"
#include "framebuffer.hxx"

namespace Kengine::graphics::render_manager
{
    void set_clear_color(const vec4& col); // set clear color

    void push_framebuffer(framebuffer& frame);
    void pop_framebuffer();
} // namespace Kengine::graphics::render_manager