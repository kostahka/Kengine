#include "Kengine/components/component.hxx"

namespace Kengine
{
    component::component(std::string_view name)
    {
        name_id = hash_string(name.data());
    }

    component::~component() {}
} // namespace Kengine