#include "Kengine/components/render-component.hxx"

namespace Kengine
{
    render_component::render_component()
        : component(component_type::render)
        , material(nullptr)
    {
        vao = std::make_shared<graphics::vertex_array>();
    }

    render_component::render_component(
        const res_ptr<material_resource>&              material,
        const std::shared_ptr<graphics::vertex_array>& vao)
        : material(material)
        , vao(vao)
        , component(component_type::render)
    {
        this->material->take_data();
    }

    std::size_t render_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, vertices_count);
        size += serialization::write(os, vertices_start);
        size += serialization::write(os, draw_mode);
        size += serialization::write(os, material);

        return size;
    }

    std::size_t render_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, vertices_count);
        size += serialization::read(is, vertices_start);
        size += serialization::read(is, draw_mode);
        size += serialization::read(is, material);

        return size;
    }

    render_component::~render_component()
    {
        material->free_data();
    }
} // namespace Kengine