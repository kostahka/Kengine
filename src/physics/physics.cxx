#include "physics.hxx"

#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/scene/scene-manager.hxx"

namespace Kengine::physics
{
    b2World* main_world;

    static int velocity_iterations = 10;
    static int position_iterations = 8;

    void initialize(vec2 gravity)
    {
        main_world = new b2World(gravity);
    }

    void shutdown()
    {
        delete main_world;
    }

    void update(int delta_ms)
    {
        scene* current_scene = scene_manager::get_current_scene();
        auto   physics_view  = current_scene->registry
                                .view<transform_component, physics_component>();

        for (auto [ent, ent_transform, ent_physics] : physics_view.each())
        {
            ent_physics->SetTransform(ent_transform.position,
                                      ent_transform.angle);
        }

        main_world->Step(
            delta_ms / 1000.0f, velocity_iterations, position_iterations);

        for (auto [ent, ent_transform, ent_physics] : physics_view.each())
        {
            auto new_position = ent_physics->GetPosition();
            auto new_angle    = ent_physics->GetAngle();

            ent_transform.position.x = new_position.x;
            ent_transform.position.y = new_position.y;
            ent_transform.angle      = new_angle;
        }
    }
} // namespace Kengine::physics