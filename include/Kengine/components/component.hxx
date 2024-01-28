#pragma once

#include "Kengine/components/component-types.hxx"
#include "Kengine/event/event.hxx"

#include "Kengine/serialization/serialization.hxx"

namespace Kengine
{
    class component : public serializable
    {
    public:
        component(component_type type);
        ~component();

        inline component_type get_type() const { return type; };

    private:
        component_type type;
    };
} // namespace Kengine