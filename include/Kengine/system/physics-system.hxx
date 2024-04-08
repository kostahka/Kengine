#pragma once

#include "Kengine/scene/scene.hxx"
#include "Kengine/system/system.hxx"

namespace Kengine
{
    class E_DECLSPEC physics_system : public system
    {
    public:
        static constexpr auto name = "physics_system";

        physics_system(scene& sc);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        void on_update(scene&, int delta_ms) override;

    private:
        scene& sc;
    };
} // namespace Kengine