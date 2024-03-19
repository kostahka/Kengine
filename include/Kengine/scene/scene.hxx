#pragma once

#include "Kengine/graphics/graphics.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "Kengine/system/system.hxx"

#include "box2d/b2_world.h"
#include "entt/entt.hpp"

#include <vector>

namespace Kengine
{
    class game;
    struct camera_component;

    class scene : public serializable
    {
    public:
        scene(game*);

        void on_update(int delta_ms);
        void on_render(int delta_ms);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        void set_game(game*);
        void set_camera(camera_component*);

        void add_system(string_id name_id);
        void remove_system(string_id name_id);

        void add_resource(res_ptr<resource>);
        void remove_resource(string_id);
        void clear_resources();

        inline const game* get_game() const { return scene_game; }

        inline const camera_component* get_camera() const
        {
            return current_camera;
        }

        inline b2World& get_world() const { return *scene_world; }

        entt::registry registry;
        vec4           clear_color;

        inline const auto& get_systems() const { return systems; }

        inline const auto& get_resources() const { return resources; }

    private:
        std::unordered_map<string_id, res_ptr<resource>>       resources;
        std::unordered_map<string_id, std::shared_ptr<system>> systems;
        std::vector<system*>                                   render_systems;
        std::vector<system*>                                   update_systems;

        camera_component* current_camera;
        game*             scene_game;
        b2World*          scene_world;
    };
} // namespace Kengine
