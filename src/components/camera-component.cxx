#include "Kengine/components/camera-component.hxx"

#include "Kengine/scene/scene.hxx"

namespace Kengine
{
    camera_component::camera_component()
        : component(component_type::camera)
        , camera()
    {
    }

    camera_component::camera_component(mat4x4 projection)
        : component(component_type::camera)
    {
        camera.set_projection(projection);
    }

    camera_component::~camera_component()
    {
        if (binded)
            cam_scene->set_camera(nullptr);
    }

    void camera_component::bind(scene& sc)
    {
        cam_scene = &sc;
        cam_scene->set_camera(this);
    }

    std::size_t camera_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, camera);
        size += serialization::write(os, binded);

        return size;
    }

    std::size_t camera_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, camera);
        size += serialization::read(is, binded);

        return size;
    }
} // namespace Kengine