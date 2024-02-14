#include "Kengine/system/system.hxx"

#include <unordered_map>

namespace Kengine
{
    render_system::render_system(std::string_view name)
    {
        name_id = hash_string(name.data());
    }

    update_system::update_system(std::string_view name)
    {
        name_id = hash_string(name.data());
    }

    void system_container::register_render_system(
        std::shared_ptr<render_system> system)
    {
        auto name_id            = system->get_name_id();
        render_systems[name_id] = system;
    }

    void system_container::register_render_system(
        std::shared_ptr<update_system> system)
    {
        auto name_id            = system->get_name_id();
        update_systems[name_id] = system;
    }

    std::shared_ptr<render_system> system_container::get_render_system(
        string_id name_id)
    {
        auto system_it = render_systems.find(name_id);
        if (system_it != render_systems.end())
        {
            return system_it->second;
        }

        return nullptr;
    }

    std::shared_ptr<update_system> system_container::get_update_system(
        string_id name_id)
    {
        auto system_it = update_systems.find(name_id);
        if (system_it != update_systems.end())
        {
            return system_it->second;
        }

        return nullptr;
    }
} // namespace Kengine