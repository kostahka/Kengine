#include "Kengine/components/component-container.hxx"

#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"

namespace Kengine
{
    component_container::component_container()
    {
        register_component(hash_string(camera_component::name),
                           camera_component::info);
        register_component<physics_component>(physics_component::name);
        register_component<render_component>(render_component::name);
        register_component<transform_component>(transform_component::name);
        register_component<sprite_component>(sprite_component::name);
    }

    void component_container::register_component(string_id             name_id,
                                                 const component_info& c_info)
    {
        component_infos[name_id] = c_info;
    }

    component_info component_container::get_component_info(string_id name_id)
    {
        auto c_info_it = component_infos.find(name_id);
        if (c_info_it != component_infos.end())
        {
            return c_info_it->second;
        }

        return component_info();
    }
} // namespace Kengine