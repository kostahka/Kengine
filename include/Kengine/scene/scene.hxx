#pragma once

#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/resources/resource-link.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "entt/entt.hpp"

#include <vector>

namespace Kengine
{
    class scene : public serializable
    {
    public:
        scene();

        void on_update(int delta_ms);
        void on_render(int delta_ms);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        entt::registry registry;

    private:
        std::vector<resource_link> resource_links;
    };
} // namespace Kengine
