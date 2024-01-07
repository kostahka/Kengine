#pragma once

#include "Kengine/units/matrix.hxx"
#include "Kengine/units/vector.hxx"


namespace Kengine::graphics::render_manager
{
    extern vec4 clear_color;

    [[nodiscard]] bool initialize();

    void update_viewport();
    void begin_render();
    void end_render();
    void update_matrices(const mat4x4& projection, const mat4x4& view);
    void update_projection(const mat4x4& projection);
    void update_view(const mat4x4& view);
} // namespace Kengine::graphics::render_manager