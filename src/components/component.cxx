#include "Kengine/components/component.hxx"

#include "Kengine/string/string-id.hxx"

namespace Kengine
{
    component::component(std::string_view name)
    {
        name_id = hash_string(name.data());
    }

    component::~component() {}
} // namespace Kengine