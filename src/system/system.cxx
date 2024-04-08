#include "Kengine/system/system.hxx"

#include "Kengine/string/string-id.hxx"
#include "Kengine/system/audio-system.hxx"
#include "Kengine/system/graphics-system.hxx"
#include "Kengine/system/gui-system.hxx"
#include "Kengine/system/physics-system.hxx"

#include <unordered_map>

namespace Kengine
{
    system::system(std::string_view name, uint8_t type_flags)
        : type_flags(type_flags)
    {
        name_id = hash_string(name.data());
    }

    void system::on_start(scene&) {}

    void system::on_render(scene&, int delta_ms) {}

    void system::on_update(scene&, int delta_ms) {}

    void system::on_event(scene&, event::game_event) {}

    void system_container::register_system(string_id        name_id,
                                           create_system_fp system_factory)
    {
        system_factories[name_id] = system_factory;
    }

    create_system_fp system_container::get_system_factory(string_id name_id)
    {
        auto system_it = system_factories.find(name_id);
        if (system_it != system_factories.end())
        {
            return system_it->second;
        }

        return nullptr;
    }

    system_container::system_container()
    {
        register_system<graphics_system>(graphics_system::name);
        register_system<physics_system>(physics_system::name);
        register_system<gui_system>(gui_system::name);
        register_system<audio_system>(audio_system::name);
    }
} // namespace Kengine