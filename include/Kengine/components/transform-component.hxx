#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/units/vector.hxx"

namespace Kengine
{
    struct transform_component : public component
    {
        transform_component();
        transform_component(const vec3& position);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        vec3  position = { 0, 0, 0 };
        float angle    = 0;
        vec3  scale    = { 1, 1, 1 };
    };
} // namespace Kengine