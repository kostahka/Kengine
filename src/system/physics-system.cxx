#include "Kengine/system/physics-system.hxx"

#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{
    static int velocity_iterations = 10;
    static int position_iterations = 8;

    physics_system::physics_system()
        : system(name, system_update_type)
    {
    }

    void physics_system::on_update(scene& sc, int delta_ms)
    {
        auto physics_view =
            sc.registry.view<transform_component, physics_component>();

        for (auto [ent, ent_transform, ent_physics] : physics_view.each())
        {
            auto local_transform = ent_transform.transform;
            ent_physics->SetTransform(local_transform.position,
                                      local_transform.angle);
        }

        sc.get_world().Step(
            delta_ms / 1000.0f, velocity_iterations, position_iterations);

        for (auto [ent, ent_transform, ent_physics] : physics_view.each())
        {
            auto new_position = ent_physics->GetPosition();
            auto new_angle    = ent_physics->GetAngle();

            ent_transform.transform.position.x = new_position.x;
            ent_transform.transform.position.y = new_position.y;
            ent_transform.transform.angle      = new_angle;
        }
    }

    std::size_t physics_system::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t physics_system::deserialize(std::istream& is)
    {
        return 0;
    }

    std::size_t physics_system::serialize_size() const
    {
        return 0;
    }
} // namespace Kengine