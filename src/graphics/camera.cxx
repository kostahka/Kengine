#include "Kengine/graphics/camera.hxx"

#include "../scene/scene-manager.hxx"
#include "graphics.hxx"

namespace Kengine::graphics
{
    camera::camera()
        : projection(1)
        , view(1)
    {
    }

    camera::~camera() {}

    void camera::set_matrices(const mat4x4& projection, const mat4x4& view)
    {
        this->projection = projection;
        this->view       = view;
    }

    void camera::set_projection(const mat4x4& projection)
    {
        this->projection = projection;
    }

    void camera::set_view(const mat4x4& view)
    {
        this->view = view;
    }

    std::size_t camera::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, projection[0].x);
        size += serialization::write(os, projection[0].y);
        size += serialization::write(os, projection[0].z);
        size += serialization::write(os, projection[0].w);
        size += serialization::write(os, projection[1].x);
        size += serialization::write(os, projection[1].y);
        size += serialization::write(os, projection[1].z);
        size += serialization::write(os, projection[1].w);
        size += serialization::write(os, projection[2].x);
        size += serialization::write(os, projection[2].y);
        size += serialization::write(os, projection[2].z);
        size += serialization::write(os, projection[2].w);
        size += serialization::write(os, projection[3].x);
        size += serialization::write(os, projection[3].y);
        size += serialization::write(os, projection[3].z);
        size += serialization::write(os, projection[3].w);
        size += serialization::write(os, view[0].x);
        size += serialization::write(os, view[0].y);
        size += serialization::write(os, view[0].z);
        size += serialization::write(os, view[0].w);
        size += serialization::write(os, view[1].x);
        size += serialization::write(os, view[1].y);
        size += serialization::write(os, view[1].z);
        size += serialization::write(os, view[1].w);
        size += serialization::write(os, view[2].x);
        size += serialization::write(os, view[2].y);
        size += serialization::write(os, view[2].z);
        size += serialization::write(os, view[2].w);
        size += serialization::write(os, view[3].x);
        size += serialization::write(os, view[3].y);
        size += serialization::write(os, view[3].z);
        size += serialization::write(os, view[3].w);

        return size;
    }

    std::size_t camera::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, projection[0].x);
        size += serialization::read(is, projection[0].y);
        size += serialization::read(is, projection[0].z);
        size += serialization::read(is, projection[0].w);
        size += serialization::read(is, projection[1].x);
        size += serialization::read(is, projection[1].y);
        size += serialization::read(is, projection[1].z);
        size += serialization::read(is, projection[1].w);
        size += serialization::read(is, projection[2].x);
        size += serialization::read(is, projection[2].y);
        size += serialization::read(is, projection[2].z);
        size += serialization::read(is, projection[2].w);
        size += serialization::read(is, projection[3].x);
        size += serialization::read(is, projection[3].y);
        size += serialization::read(is, projection[3].z);
        size += serialization::read(is, projection[3].w);
        size += serialization::read(is, view[0].x);
        size += serialization::read(is, view[0].y);
        size += serialization::read(is, view[0].z);
        size += serialization::read(is, view[0].w);
        size += serialization::read(is, view[1].x);
        size += serialization::read(is, view[1].y);
        size += serialization::read(is, view[1].z);
        size += serialization::read(is, view[1].w);
        size += serialization::read(is, view[2].x);
        size += serialization::read(is, view[2].y);
        size += serialization::read(is, view[2].z);
        size += serialization::read(is, view[2].w);
        size += serialization::read(is, view[3].x);
        size += serialization::read(is, view[3].y);
        size += serialization::read(is, view[3].z);
        size += serialization::read(is, view[3].w);

        return size;
    }
} // namespace Kengine::graphics