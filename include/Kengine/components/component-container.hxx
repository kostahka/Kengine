#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/string/string-id.hxx"

#include <unordered_map>

namespace Kengine
{
    class E_DECLSPEC component_container
    {
    public:
        component_container();

        void           register_component(string_id             name_id,
                                          const component_info& c_info);
        component_info get_component_info(string_id name_id);

        template <typename ComponentType>
        void register_component(std::string_view name)
        {
            static_assert(std::is_base_of_v<component, ComponentType>,
                          "ComponentType only inhertis component");

            auto c_info  = create_component_info<ComponentType>();
            auto name_id = hash_string(name.data());
            register_component(name_id, c_info);
        }

        inline const auto& get_component_infos() const
        {
            return component_infos;
        }

    private:
        std::unordered_map<string_id, component_info> component_infos;
    };
} // namespace Kengine