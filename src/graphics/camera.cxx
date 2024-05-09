#include "Kengine/graphics/camera.hxx"

#include "../scene/scene-manager.hxx"
#include "graphics.hxx"

namespace Kengine::graphics
{
    camera::camera()
        : view(1)
        , height(1)
        , zNear(-1)
        , zFar(1)
    {
    }

    camera::~camera() {}

    void camera::set_projection(float height, float zNear, float zFar)
    {
        this->height     = height;
        this->zNear      = zNear;
        this->zFar       = zFar;
        projection_valid = false;
    }

    void camera::set_height(float height)
    {
        this->height     = height;
        projection_valid = false;
    }

    void camera::calculate_projection()
    {
        auto current_viewport = graphics::get_current_viewport();
        auto width =
            height / float(current_viewport.y) * float(current_viewport.x);
        projection = glm::ortho(
            -width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
        projection_valid = true;
        invProjection    = glm::inverse(projection);
    }

    std::size_t camera::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, height);
        size += serialization::write(os, zNear);
        size += serialization::write(os, zFar);

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

        size += serialization::read(is, height);
        size += serialization::read(is, zNear);
        size += serialization::read(is, zFar);

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

        projection_valid = false;

        return size;
    }

    std::size_t camera::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(height);
        size += serialization::size(zNear);
        size += serialization::size(zFar);

        size += serialization::size(view[0].x);
        size += serialization::size(view[0].y);
        size += serialization::size(view[0].z);
        size += serialization::size(view[0].w);
        size += serialization::size(view[1].x);
        size += serialization::size(view[1].y);
        size += serialization::size(view[1].z);
        size += serialization::size(view[1].w);
        size += serialization::size(view[2].x);
        size += serialization::size(view[2].y);
        size += serialization::size(view[2].z);
        size += serialization::size(view[2].w);
        size += serialization::size(view[3].x);
        size += serialization::size(view[3].y);
        size += serialization::size(view[3].z);
        size += serialization::size(view[3].w);

        return size;
    }

    vec2 camera::screen_to_world(const vec2& point)
    {
        auto viewport = graphics::get_current_viewport();

        vec4 ndc_pos = {
            point.x / viewport.x, point.y / viewport.y, 0.0f, 1.0f
        };
        vec4 view_pos  = invProjection * ndc_pos;
        vec4 world_pos = glm::inverse(view) * view_pos;

        return { world_pos.x, world_pos.y };
    }

} // namespace Kengine::graphics