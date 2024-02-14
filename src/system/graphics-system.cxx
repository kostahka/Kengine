#include "Kengine/system/graphics-system.hxx"

#include "../graphics/graphics.hxx"

namespace Kengine
{

    graphics_system::graphics_system()
        : render_system(name)
        , update_system(name)
    {
    }

    void graphics_system::on_render(scene& sc, int delta_ms)
    {
        graphics::on_render(sc, delta_ms);
    }

    void graphics_system::on_update(scene& sc, int delta_ms)
    {
        graphics::on_update(sc, delta_ms);
    }
} // namespace Kengine