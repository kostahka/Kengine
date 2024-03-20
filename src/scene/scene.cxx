#include "Kengine/scene/scene.hxx"

#include "../graphics/graphics.hxx"
#include "../physics/physics.hxx"
#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/game.hxx"
#include "Kengine/serialization/archive.hxx"

namespace Kengine
{
    static vec2 gravity = { 0, 9.81f };

    scene::scene(game* sc_game)
        : scene_game(sc_game)
        , current_camera(graphics::default_camera_component)
        , clear_color(0, 0, 0, 0)
    {
        scene_world = new b2World(gravity);
    };

    std::size_t scene::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        uint32_t systems_count = static_cast<uint32_t>(systems.size());
        size += serialization::write(os, systems_count);
        for (auto& system : systems)
        {
            auto name_id = system.first;
            size += serialization::write(os, name_id);
            size += serialization::write(os, *(system.second));
        }

        archive_output output(os, *this);

        entt::snapshot snapshot{ registry };

        snapshot.get<entt::entity>(output);

        for (auto& c_info_it : scene_game->get_component_infos())
        {
            c_info_it.second.serialize_component(snapshot, output);
        }

        size += output.get_size();

        size += serialization::write(os, clear_color.r);
        size += serialization::write(os, clear_color.g);
        size += serialization::write(os, clear_color.b);
        size += serialization::write(os, clear_color.a);

        return size;
    }

    std::size_t scene::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        systems.clear();
        render_systems.clear();
        update_systems.clear();

        uint32_t systems_count = 0;
        size += serialization::read(is, systems_count);
        systems.reserve(systems_count);
        for (auto i = 0U; i < systems_count; i++)
        {
            string_id name_id{};
            size += serialization::read(is, name_id);

            if (scene_game)
            {
                auto system_factory = scene_game->get_system_factory(name_id);
                auto system         = system_factory();
                serialization::read(is, *system);
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
                system->on_create(*this);
            }
        }

        registry.clear();

        archive_input input(is, *this);

        entt::snapshot_loader snapshot{ registry };

        snapshot.get<entt::entity>(input);

        for (auto& c_info_it : scene_game->get_component_infos())
        {
            c_info_it.second.deserialize_component(snapshot, input);
        }

        snapshot.orphans();

        size += input.get_size();

        size += serialization::read(is, clear_color.r);
        size += serialization::read(is, clear_color.g);
        size += serialization::read(is, clear_color.b);
        size += serialization::read(is, clear_color.a);

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
        graphics::update_matrices(current_camera->camera.get_projection(),
                                  current_camera->camera.view);
        for (auto& system : render_systems)
        {
            system->on_render(*this, delta_ms);
        }
    }

    void scene::set_game(game* game)
    {
        scene_game = game;
    }

    void scene::set_camera(camera_component* cam)
    {
        if (cam)
        {
            if (current_camera)
            {
                current_camera->binded = false;
            }
            current_camera         = cam;
            current_camera->binded = true;
        }
        else
        {
            current_camera = graphics::default_camera_component;
        }
    }

    void scene::add_system(string_id name_id)
    {
        remove_system(name_id);
        auto system_factory = scene_game->get_system_factory(name_id);
        if (system_factory)
        {
            auto system      = system_factory();
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
            system->on_create(*this);
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
} // namespace Kengine