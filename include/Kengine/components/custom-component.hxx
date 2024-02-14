#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/string/string-id.hxx"

#include <string_view>

namespace Kengine
{
    void registrate_component(const component_info& c_info);

    class custom_component : public component
    {
    public:
        custom_component(std::string_view component_name);

    private:
        string_id component_id;
    };
} // namespace Kengine