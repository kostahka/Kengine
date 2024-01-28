#include "Kengine/components/sprite-component.hxx"

#include "../graphics/graphics.hxx"

namespace Kengine
{
    sprite_component::sprite_component()
        : component(component_type::sprite)
        , ent(entt::null)
        , material(nullptr)
    {
    }

    sprite_component::sprite_component(
        entt::entity                             ent,
        const res_ptr<sprite_material_resource>& material,
        rect                                     uv,
        vec2                                     origin)
        : component(component_type::sprite)
        , ent(ent)
        , material(material)
        , uv(uv)
        , origin(origin)
    {
        graphics::add_sprite(this);
        this->material->take_data();
    }

    sprite_component::~sprite_component()
    {
        material->free_data();
        graphics::remove_sprite(this);
    }

    std::size_t sprite_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, origin.x);
        size += serialization::write(os, origin.y);
        size += serialization::write(os, uv.x);
        size += serialization::write(os, uv.y);
        size += serialization::write(os, uv.w);
        size += serialization::write(os, uv.h);
        size += serialization::write(os, ent);
        size += serialization::write(os, material);

        return size;
    }

    std::size_t sprite_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, origin.x);
        size += serialization::read(is, origin.y);
        size += serialization::read(is, uv.x);
        size += serialization::read(is, uv.y);
        size += serialization::read(is, uv.w);
        size += serialization::read(is, uv.h);
        size += serialization::read(is, ent);
        size += serialization::read(is, material);

        return size;
    }
} // namespace Kengine