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

    void camera::set_projection(float width,
                                float height,
                                float zNear,
                                float zFar)
    {
        projection = glm::ortho(
            -width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
        this->width  = width;
        this->height = height;
        this->zNear  = zNear;
        this->zFar   = zFar;
    }

    std::size_t camera::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, width);
        size += serialization::write(os, height);
        size += serialization::write(os, zNear);
        size += serialization::write(os, zFar);

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

        size += serialization::read(is, width);
        size += serialization::read(is, height);
        size += serialization::read(is, zNear);
        size += serialization::read(is, zFar);

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