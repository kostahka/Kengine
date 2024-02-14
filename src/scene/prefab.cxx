#include "Kengine/scene/prefab.hxx"

namespace Kengine
{
    prefab::prefab() {}

    std::size_t prefab::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t prefab::deserialize(std::istream& is)
    {
        return 0;
    }
} // namespace Kengine