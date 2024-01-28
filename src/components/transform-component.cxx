#include "Kengine/components/transform-component.hxx"

namespace Kengine
{
    transform_component::transform_component()
        : component(component_type::transform){};

    transform_component::transform_component(const vec3& position)
        : component(component_type::transform)
        , position(position){};

    std::size_t transform_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, position.x);
        size += serialization::write(os, position.y);
        size += serialization::write(os, position.z);
        size += serialization::write(os, angle);
        size += serialization::write(os, scale.x);
        size += serialization::write(os, scale.y);
        size += serialization::write(os, scale.z);

        return size;
    }

    std::size_t transform_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, position.x);
        size += serialization::read(is, position.y);
        size += serialization::read(is, position.z);
        size += serialization::read(is, angle);
        size += serialization::read(is, scale.x);
        size += serialization::read(is, scale.y);
        size += serialization::read(is, scale.z);

        return size;
    }
} // namespace Kengine