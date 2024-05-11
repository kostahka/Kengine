#pragma once

#include "Kengine/graphics/graphics.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/system/system.hxx"

#include "box2d/b2_world.h"
#include "entt/entt.hpp"

#include <vector>

namespace entt
{
    typedef decltype(entt::registry().view<entt::entity>().begin())
        entity_iterator;
}

namespace Kengine
{
    class game;
    struct camera_component;

    class E_DECLSPEC scene : public serializable
    {
    public:
        scene(game*);

        void on_start();

        void on_update(int delta_ms);
        void on_render(int delta_ms);
        void on_event(const event::game_event&);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        std::size_t deserialize_continuous(std::istream& is);

        void instansiate(std::shared_ptr<scene>);
        void instansiate(std::filesystem::path prefab_path);
        void instansiate(string_id prefab_id);

        void set_game(game*);

        void add_system(string_id name_id);
        void remove_system(string_id name_id);

        void add_resource(res_ptr<resource>);
        void remove_resource(string_id);
        void clear_resources();

        void set_camera(graphics::camera*);
        void set_camera(entt::entity);

        inline const game* get_game() const { return scene_game; }

        inline graphics::camera& get_camera() const { return *current_camera; }

        inline entt::entity get_camera_entity() const { return camera_entity; }

        inline b2World& get_world() const { return *scene_world; }

        entt::registry registry;
        vec4           clear_color;

        inline const auto& get_systems() const { return systems; }

        inline const auto& get_resources() const { return resources; }

    private:
        void on_construct_entity(entt::entity ent);
        void on_destroy_camera(entt::entity ent);

        void on_update_rect_transform(entt::entity ent);

        std::unordered_map<string_id, res_ptr<resource>>       resources;
        std::unordered_map<string_id, std::shared_ptr<system>> systems;
        std::vector<system*>                                   render_systems;
        std::vector<system*>                                   update_systems;
        std::vector<system*>                                   event_systems;

        entt::entity      camera_entity  = entt::null;
        graphics::camera* current_camera = nullptr;
        graphics::camera  last_camera{};
        game*             scene_game  = nullptr;
        b2World*          scene_world = nullptr;
    };
} // namespace Kengine
