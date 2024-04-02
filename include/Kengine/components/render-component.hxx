#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/resources/material-resource.hxx"

namespace Kengine
{
    struct render_component : public component
    {
        static constexpr auto name = "render_component";

        render_component();
        render_component(const res_ptr<material_resource>& material,
                         const std::shared_ptr<graphics::vertex_array>& vao);
        ~render_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        uint32_t                   vertices_count = 0;
        uint32_t                   vertices_start = 0;
        graphics::draw_mode        draw_mode = graphics::draw_mode::triangles;
        res_ptr<material_resource> material;
        std::shared_ptr<graphics::vertex_array> vao;
    };
} // namespace Kengine