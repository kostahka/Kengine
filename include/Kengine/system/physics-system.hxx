#pragma once

#include "Kengine/system/system.hxx"

namespace Kengine
{
    class physics_system : public update_system
    {
    public:
        static constexpr auto name = "physics_system";

        physics_system();

        void on_update(scene&, int delta_ms) override;
    };
} // namespace Kengine