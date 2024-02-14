#include "custom-component.hxx"

#include "Kengine/log/log.hxx"

namespace Kengine
{
    std::vector<component_info> custom_component_infos;

    void registrate_component(const component_info& c_info)
    {
        KENGINE_ASSERT(c_info.deserialize_component,
                       "deserialize info must not be null");
        KENGINE_ASSERT(c_info.serialize_component,
                       "serialize info must not be null");

        if (c_info.deserialize_component && c_info.serialize_component)
            custom_component_infos.push_back(c_info);
    }

    custom_component::custom_component(std::string_view component_name)
        : component(component_type::custom)
    {
    }
} // namespace Kengine