#pragma once

#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/resources/material-resource.hxx"

namespace Kengine::graphics
{
    class E_DECLSPEC render_packet
    {
    public:
        explicit render_packet(const res_ptr<material_resource>& material);
        ~render_packet();

        render_packet(const render_packet& other);
        render_packet(render_packet&& other);

        render_packet& operator=(const render_packet& other);
        render_packet& operator=(render_packet&& other);

        std::shared_ptr<vertex_array> vao             = nullptr;
        int                           layer           = 0;
        draw_mode                     mode            = draw_mode::triangles;
        uint32_t                      instances_count = 1;
        uint32_t                      vertices_count  = 3;
        uint32_t                      vertices_start  = 0;

        inline res_ptr<material_resource>& get_material() { return material; }

        inline const res_ptr<material_resource>& get_material() const
        {
            return material;
        }

    private:
        res_ptr<material_resource> material = nullptr;
    };
} // namespace Kengine::graphics
