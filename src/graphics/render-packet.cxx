#include "Kengine/graphics/render-packet.hxx"

namespace Kengine::graphics
{
    render_packet::render_packet(const res_ptr<material_resource>& material)
        : material(material)
    {
        if (material)
        {
            this->material->take_data();
        }
    }

    render_packet::~render_packet()
    {
        if (material)
        {
            material->free_data();
        }
    }

    render_packet::render_packet(const render_packet& other)
    {
        material = other.material;
        if (material)
        {
            material->take_data();
        }
        vao             = other.vao;
        layer           = other.layer;
        mode            = other.mode;
        instances_count = other.instances_count;
        vertices_count  = other.vertices_count;
        vertices_start  = other.vertices_start;
    }

    render_packet::render_packet(render_packet&& other)
    {
        material        = other.material;
        other.material  = nullptr;
        vao             = other.vao;
        layer           = other.layer;
        mode            = other.mode;
        instances_count = other.instances_count;
        vertices_count  = other.vertices_count;
        vertices_start  = other.vertices_start;
    }

    render_packet& render_packet::operator=(const render_packet& other)
    {
        if (material)
        {
            material->free_data();
        }
        material = other.material;
        if (material)
        {
            material->take_data();
        }
        vao             = other.vao;
        layer           = other.layer;
        mode            = other.mode;
        instances_count = other.instances_count;
        vertices_count  = other.vertices_count;
        vertices_start  = other.vertices_start;

        return *this;
    }

    render_packet& render_packet::operator=(render_packet&& other)
    {
        if (material)
        {
            material->free_data();
        }
        material        = other.material;
        other.material  = nullptr;
        vao             = other.vao;
        layer           = other.layer;
        mode            = other.mode;
        instances_count = other.instances_count;
        vertices_count  = other.vertices_count;
        vertices_start  = other.vertices_start;

        return *this;
    }
} // namespace Kengine::graphics