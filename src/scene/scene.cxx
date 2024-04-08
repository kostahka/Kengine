#include "Kengine/scene/scene.hxx"

#include "../graphics/graphics.hxx"
#include "../physics/physics.hxx"
#include "../scene/scene-manager.hxx"
#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/game.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/serialization/archive.hxx"

namespace Kengine
{
    static vec2 gravity = { 0, 9.81f };

    scene::scene(game* sc_game)
        : scene_game(sc_game)
        , current_camera(graphics::default_camera)
        , clear_color(0, 0, 0, 0)
    {
        scene_world = new b2World(gravity);
        registry.on_construct<entt::entity>()
            .connect<&scene::on_construct_entity>(*this);
        registry.on_destroy<camera_component>()
            .connect<&scene::on_destroy_camera>(*this);
    };

    void scene::on_start()
    {
        for (auto& system : systems)
        {
            system.second->on_start(*this);
        }
    }

    void scene::on_construct_entity(entt::entity ent) {}

    void scene::on_destroy_camera(entt::entity ent)
    {
        if (ent == camera_entity)
        {
            camera_entity  = entt::null;
            current_camera = graphics::default_camera;
        }
    }

    std::size_t scene::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        uint32_t systems_count = static_cast<uint32_t>(systems.size());
        size += serialization::write(os, systems_count);
        for (auto& system : systems)
        {
            auto name_id = system.first;
            size += serialization::write(os, name_id);

            size_t system_size = serialization::size(*system.second);
            size += serialization::write(os, system_size);

            size += serialization::write(os, *(system.second));
        }

        archive_output output(os, *this);

        entt::snapshot snapshot{ registry };

        snapshot.get<entt::entity>(output);

        auto component_infos = scene_game->get_component_infos();

        size_t component_count = component_infos.size();
        size += serialization::write(os, component_infos.size());

        for (auto& c_info_it : scene_game->get_component_infos())
        {
            size += serialization::write(os, c_info_it.first);

            archive_size component_size_out(*this);
            c_info_it.second.serialize_size_component(snapshot,
                                                      component_size_out);
            size_t component_size = component_size_out.get_size();
            size += serialization::write(os, component_size);

            c_info_it.second.serialize_component(snapshot, output);
        }

        size += output.get_size();

        size += serialization::write(os, clear_color.r);
        size += serialization::write(os, clear_color.g);
        size += serialization::write(os, clear_color.b);
        size += serialization::write(os, clear_color.a);

        size += serialization::write(os, camera_entity);

        return size;
    }

    std::size_t scene::deserialize(std::istream& is)
    {
        KENGINE_ASSERT(scene_game, "No game on scene");
        if (!scene_game)
        {
            return 0;
        }

        std::size_t size = 0;

        systems.clear();
        render_systems.clear();
        update_systems.clear();
        event_systems.clear();

        uint32_t systems_count = 0;
        size += serialization::read(is, systems_count);
        systems.reserve(systems_count);
        for (auto i = 0U; i < systems_count; i++)
        {
            string_id name_id{};
            size += serialization::read(is, name_id);
            size_t system_size = 0;
            size += serialization::read(is, system_size);

            auto system_factory = scene_game->get_system_factory(name_id);
            if (system_factory)
            {
                auto system = system_factory(*this);
                size += serialization::read(is, *system);
                systems[name_id] = system;

                auto system_type_flags = system->get_type_flags();
                if (system_type_flags & system_render_type)
                {
                    render_systems.push_back(system.get());
                }
                if (system_type_flags & system_update_type)
                {
                    update_systems.push_back(system.get());
                }
                if (system_type_flags & system_event_type)
                {
                    event_systems.push_back(system.get());
                }
            }
            else
            {
                is.seekg(system_size, std::ios_base::cur);
            }
        }

        registry.on_construct<entt::entity>()
            .disconnect<&scene::on_construct_entity>(*this);
        registry.clear();

        archive_input input(is, *this);

        entt::snapshot_loader snapshot{ registry };

        snapshot.get<entt::entity>(input);

        auto component_infos = scene_game->get_component_infos();

        size_t component_count = 0;
        size += serialization::read(is, component_count);

        for (auto i = 0U; i < component_count; ++i)
        {
            string_id component_name;
            size += serialization::read(is, component_name);
            size_t component_size = 0;
            size += serialization::read(is, component_size);

            auto component_info_it = component_infos.find(component_name);
            if (component_info_it == component_infos.end())
            {
                is.seekg(component_size, std::ios_base::cur);
            }
            else
            {
                component_info_it->second.deserialize_component(snapshot,
                                                                input);
            }
        }

        snapshot.orphans();
        registry.on_construct<entt::entity>()
            .connect<&scene::on_construct_entity>(*this);

        size += input.get_size();

        size += serialization::read(is, clear_color.r);
        size += serialization::read(is, clear_color.g);
        size += serialization::read(is, clear_color.b);
        size += serialization::read(is, clear_color.a);

        size += serialization::read(is, camera_entity);

        set_camera(camera_entity);

        return size;
    }

    std::size_t scene::serialize_size() const
    {
        std::size_t size = 0;

        uint32_t systems_count = static_cast<uint32_t>(systems.size());
        size += serialization::size(systems_count);
        for (auto& system : systems)
        {
            auto name_id = system.first;
            size += serialization::size(name_id);

            size_t system_size = serialization::size(*system.second);
            size += serialization::size(system_size);

            size += serialization::size(*(system.second));
        }

        archive_size output(*this);

        entt::snapshot snapshot{ registry };

        snapshot.get<entt::entity>(output);

        auto component_infos = scene_game->get_component_infos();

        size_t component_count = component_infos.size();
        size += serialization::size(component_infos.size());

        for (auto& c_info_it : scene_game->get_component_infos())
        {
            size += serialization::size(c_info_it.first);

            archive_size component_size_out(*this);
            c_info_it.second.serialize_size_component(snapshot,
                                                      component_size_out);
            size_t component_size = component_size_out.get_size();
            size += serialization::size(component_size);

            c_info_it.second.serialize_size_component(snapshot, output);
        }

        size += output.get_size();

        size += serialization::size(clear_color.r);
        size += serialization::size(clear_color.g);
        size += serialization::size(clear_color.b);
        size += serialization::size(clear_color.a);

        size += serialization::size(camera_entity);

        return size;
    }

    std::size_t scene::deserialize_continuous(std::istream& is)
    {
        KENGINE_ASSERT(scene_game, "No game on scene");
        if (!scene_game)
        {
            return 0;
        }

        std::size_t size = 0;

        uint32_t systems_count = 0;
        size += serialization::read(is, systems_count);
        for (auto i = 0U; i < systems_count; i++)
        {
            string_id name_id{};
            size += serialization::read(is, name_id);
            size_t system_size = 0;
            size += serialization::read(is, system_size);

            auto system_factory = scene_game->get_system_factory(name_id);
            if (system_factory && !systems.contains(name_id))
            {
                auto system = system_factory(*this);
                size += serialization::read(is, *system);
                systems[name_id] = system;

                auto system_type_flags = system->get_type_flags();
                if (system_type_flags & system_render_type)
                {
                    render_systems.push_back(system.get());
                }
                if (system_type_flags & system_update_type)
                {
                    update_systems.push_back(system.get());
                }
                if (system_type_flags & system_event_type)
                {
                    event_systems.push_back(system.get());
                }
            }
            else
            {
                is.seekg(system_size, std::ios_base::cur);
            }
        }

        registry.on_construct<entt::entity>()
            .disconnect<&scene::on_construct_entity>(*this);

        archive_input input(is, *this);

        entt::continuous_loader snapshot{ registry };

        snapshot.get<entt::entity>(input);

        auto component_infos = scene_game->get_component_infos();

        size_t component_count = 0;
        size += serialization::read(is, component_count);

        for (auto i = 0U; i < component_count; ++i)
        {
            string_id component_name;
            size += serialization::read(is, component_name);
            size_t component_size = 0;
            size += serialization::read(is, component_size);

            auto component_info_it = component_infos.find(component_name);
            if (component_info_it == component_infos.end())
            {
                is.seekg(component_size, std::ios_base::cur);
            }
            else
            {
                component_info_it->second.deserialize_continuous_component(
                    snapshot, input);
            }
        }

        snapshot.orphans();
        registry.on_construct<entt::entity>()
            .connect<&scene::on_construct_entity>(*this);

        size += input.get_size();

        return size;
    }

    void scene::on_update(int delta_ms)
    {
        for (auto& system : update_systems)
        {
            system->on_update(*this, delta_ms);
        }
    }

    void scene::on_render(int delta_ms)
    {

        for (auto& system : render_systems)
        {
            system->on_render(*this, delta_ms);
        }
    }

    void scene::on_event(event::game_event g_event)
    {
        for (auto& system : event_systems)
        {
            system->on_event(*this, g_event);
        }
    }

    void scene::set_game(game* game)
    {
        scene_game = game;
    }

    void scene::set_camera(graphics::camera* cam)
    {
        if (cam)
        {
            current_camera = cam;
        }
        else
        {
            current_camera = graphics::default_camera;
        }
    }

    void scene::set_camera(entt::entity cam_entity)
    {
        if (cam_entity != entt::null && registry.valid(cam_entity) &&
            registry.any_of<camera_component>(cam_entity))
        {
            camera_entity    = cam_entity;
            auto& ent_camera = registry.get<camera_component>(cam_entity);
            current_camera   = &ent_camera.camera;
        }
        else
        {
            camera_entity  = entt::null;
            current_camera = graphics::default_camera;
        }
    }

    void scene::add_system(string_id name_id)
    {
        remove_system(name_id);
        auto system_factory = scene_game->get_system_factory(name_id);
        if (system_factory)
        {
            auto system      = system_factory(*this);
            systems[name_id] = system;

            auto system_type_flags = system->get_type_flags();
            if (system_type_flags & system_render_type)
            {
                render_systems.push_back(system.get());
            }
            if (system_type_flags & system_update_type)
            {
                update_systems.push_back(system.get());
            }
            if (system_type_flags & system_event_type)
            {
                event_systems.push_back(system.get());
            }
        }
    }

    void scene::remove_system(string_id name_id)
    {
        auto system_it = systems.find(name_id);
        if (system_it != systems.end())
        {
            auto system_type_flags = system_it->second->get_type_flags();
            if (system_type_flags & system_render_type)
            {
                auto render_system_it = std::remove(render_systems.begin(),
                                                    render_systems.end(),
                                                    system_it->second.get());
                if (render_system_it != render_systems.end())
                {
                    render_systems.erase(render_system_it);
                }
            }
            if (system_type_flags & system_update_type)
            {
                auto update_system_it = std::remove(update_systems.begin(),
                                                    update_systems.end(),
                                                    system_it->second.get());
                if (update_system_it != update_systems.end())
                {
                    update_systems.erase(update_system_it);
                }
            }
            if (system_type_flags & system_event_type)
            {
                auto event_system_it = std::remove(event_systems.begin(),
                                                   event_systems.end(),
                                                   system_it->second.get());
                if (event_system_it != event_systems.end())
                {
                    event_systems.erase(event_system_it);
                }
            }

            systems.erase(system_it);
        }
    }

    void scene::add_resource(res_ptr<resource> res)
    {
        if (res)
        {
            resources[res->get_resource_id()] = res;
        }
    }

    void scene::remove_resource(string_id res_id)
    {
        auto res_it = resources.find(res_id);
        if (res_it != resources.end())
            resources.erase(res_it);
    }

    void scene::clear_resources()
    {
        resources.clear();
    }

    void scene::instansiate(std::shared_ptr<scene> prefab)
    {
        std::unordered_map<entt::entity, entt::entity> entity_map;

        auto ent_view = prefab->registry.view<entt::entity>();

        registry.on_construct<entt::entity>()
            .disconnect<&scene::on_construct_entity>(*this);

        for (auto [ent] : ent_view.each())
        {
            auto new_entity = registry.create();
            entity_map[ent] = new_entity;
        }

        for (auto& c_info_it : scene_game->get_component_infos())
        {
            for (auto ent_it : entity_map)
            {
                if (c_info_it.second.has_component(*prefab, ent_it.first))
                {
                    auto other_component =
                        c_info_it.second.get_component(*prefab, ent_it.first);
                    c_info_it.second.copy_component(
                        *this, ent_it.second, other_component, entity_map);
                }
            }
        }

        registry.on_construct<entt::entity>()
            .connect<&scene::on_construct_entity>(*this);
    }

    void scene::instansiate(std::filesystem::path prefab_path)
    {
        auto prefab_file = file_manager::load_file(
            scene_manager::assets_base_folder / prefab_path);
        if (prefab_file && prefab_file->size() > 0)
        {
            std::istream prefab_stream(prefab_file.get());
            deserialize_continuous(prefab_stream);
        }
    }

    void scene::instansiate(string_id prefab_id)
    {
        auto scene_links   = scene_game->get_scene_links();
        auto scene_link_it = scene_links.find(prefab_id);
        if (scene_link_it != scene_links.end())
        {
            instansiate(scene_link_it->second);
        }
    }
} // namespace Kengine