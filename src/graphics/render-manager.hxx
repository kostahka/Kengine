#pragma once

namespace Kengine::graphics::render_manager
{
    [[nodiscard]] bool initialize();

    void begin_render();
    void end_render();
} // namespace Kengine::graphics::render_manager