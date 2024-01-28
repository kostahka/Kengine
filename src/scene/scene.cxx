#include "Kengine/scene/scene.hxx"

#include "../graphics/graphics.hxx"
#include "../physics/physics.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/serialization/archive.hxx"

namespace Kengine
{

    scene::scene(){};

    std::size_t scene::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, resource_links);

        archive_output output(os);

        entt::snapshot{ registry }
            .get<transform_component>(output)
            .get<physics_component>(output)
            .get<render_component>(output)
            .get<sprite_component>(output);

        size += output.get_size();

        return size;
    }

    std::size_t scene::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, resource_links);

        archive_input input(is);

        entt::snapshot_loader{ registry }
            .get<transform_component>(input)
            .get<physics_component>(input)
            .get<render_component>(input)
            .get<sprite_component>(input)
            .orphans();

        size += input.get_size();

        return size;
    }

    void scene::on_update(int delta_ms)
    {
        physics::update(delta_ms);
    }

    void scene::on_render(int delta_ms)
    {
        graphics::on_render(delta_ms);
    }
} // namespace Kengine