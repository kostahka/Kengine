#pragma once

#include "Kengine/system/system.hxx"

namespace Kengine
{
    class graphics_system : public render_system, public update_system
    {
    public:
        static constexpr auto name = "graphics_system";

        graphics_system();

        void on_render(scene&, int delta_ms) override;
        void on_update(scene&, int delta_ms) override;
    };
} // namespace Kengine