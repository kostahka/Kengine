#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine::graphics::render_manager
{
    extern vec4 clear_color;

    [[nodiscard]] bool initialize();

    void update_viewport();
    void begin_render();
    void end_render();
} // namespace Kengine::graphics::render_manager