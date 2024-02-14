#include "Kengine/scene/scene.hxx"

#include "../components/custom-component.hxx"
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
    static camera_component default_camera{};
    static vec2             gravity = { 0, 9.81f };

    scene::scene()
        : scene_game(nullptr)
        , current_camera(&default_camera)
    {
        scene_world = new b2World(gravity);
    };

    std::size_t scene::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, resource_links);

        archive_output output(os, *this);

        entt::snapshot snapshot{ registry };
        snapshot.get<transform_component>(output)
            .get<physics_component>(output)
            .get<render_component>(output)
            .get<sprite_component>(output)
            .get<camera_component>(output);

        for (auto& c_info : custom_component_infos)
        {
            c_info.serialize_component(snapshot, output);
        }

        size += output.get_size();

        uint32_t render_systems_count =
            static_cast<uint32_t>(render_systems.size());
        size += serialization::write(os, render_systems_count);
        for (auto& r_system : render_systems)
        {
            auto name_id = r_system->get_name_id();
            size += serialization::write(os, name_id);
        }

        uint32_t update_systems_count =
            static_cast<uint32_t>(update_systems.size());
        size += serialization::write(os, update_systems_count);
        for (auto& r_system : update_systems)
        {
            auto name_id = r_system->get_name_id();
            size += serialization::write(os, name_id);
        }

        size += serialization::write(os, clear_color.r);
        size += serialization::write(os, clear_color.g);
        size += serialization::write(os, clear_color.b);
        size += serialization::write(os, clear_color.a);

        return size;
    }

    std::size_t scene::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, resource_links);

        archive_input input(is, *this);

        entt::snapshot_loader snapshot{ registry };
        snapshot.get<transform_component>(input)
            .get<physics_component>(input)
            .get<render_component>(input)
            .get<sprite_component>(input)
            .get<camera_component>(input);

        for (auto& c_info : custom_component_infos)
        {
            c_info.deserialize_component(snapshot, input);
        }

        snapshot.orphans();

        size += input.get_size();

        render_systems.clear();
        update_systems.clear();

        uint32_t render_systems_count = 0;
        size += serialization::read(is, render_systems_count);
        render_systems.reserve(render_systems_count);
        for (auto i = 0U; i < render_systems_count; i++)
        {
            string_id name_id = 0;
            size += serialization::read(is, name_id);

            if (scene_game)
            {
                auto system = scene_game->get_render_system(name_id);
                render_systems.push_back(system);
            }
        }

        uint32_t update_systems_count = 0;
        size += serialization::read(is, update_systems_count);
        update_systems.reserve(update_systems_count);
        for (auto i = 0U; i < update_systems_count; i++)
        {
            string_id name_id = 0;
            size += serialization::read(is, name_id);

            if (scene_game)
            {
                auto system = scene_game->get_update_system(name_id);
                update_systems.push_back(system);
            }
        }

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
                                  current_camera->camera.get_view());
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
            current_camera = &default_camera;
        }
    }

    void scene::add_render_system(string_id name_id)
    {
        auto system = scene_game->get_render_system(name_id);
        render_systems.push_back(system);
    }

    void scene::add_update_system(string_id name_id)
    {
        auto system = scene_game->get_update_system(name_id);
        update_systems.push_back(system);
    }

    void scene::add_resource_link(resource_link res_link)
    {
        resource_links.push_back(res_link);
    }

} // namespace Kengine