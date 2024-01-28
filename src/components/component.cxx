#include "Kengine/components/component.hxx"

namespace Kengine
{
    component::component(component_type type)
        : type(type)
    {
    }

    component::~component() {}
} // namespace Kengine