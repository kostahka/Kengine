#pragma once

#include "Kengine/system/system.hxx"

namespace Kengine
{
    class physics_system : public system
    {
    public:
        static constexpr auto name = "physics_system";

        physics_system();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        void on_update(scene&, int delta_ms) override;
    };
} // namespace Kengine