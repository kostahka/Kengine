#include "Kengine/components/component-container.hxx"

#include "Kengine/components/audio-component.hxx"
#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/gui-component.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/rect-transform-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/text-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/string/string-id.hxx"

namespace Kengine
{
    component_container::component_container()
    {
        register_component(hash_string(physics_component::name),
                           physics_component::info);
        register_component(hash_string(rect_transform_component::name),
                           rect_transform_component::info);
        register_component<render_component>(render_component::name);
        register_component<camera_component>(camera_component::name);
        register_component(hash_string(transform_component::name),
                           transform_component::info);
        register_component<sprite_component>(sprite_component::name);
        register_component<gui_component>(gui_component::name);
        register_component<text_component>(text_component::name);
        register_component<audio_component>(audio_component::name);
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