#include "Kengine/components/camera-component.hxx"

#include "Kengine/components/component-info.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

namespace Kengine
{
    camera_component::camera_component()
        : component(name)
        , camera()
    {
    }

    camera_component::camera_component(camera_component& other)
        : component(name)
        , camera(other.camera)
    {
    }

    camera_component::camera_component(camera_component&& other)
        : component(name)
        , camera(other.camera)
    {
    }

    camera_component& camera_component::operator=(camera_component& other)
    {
        camera = other.camera;
        return *this;
    }

    camera_component& camera_component::operator=(camera_component&& other)
    {
        camera = other.camera;
        return *this;
    }

    camera_component::~camera_component() {}

    std::size_t camera_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, camera);

        return size;
    }

    std::size_t camera_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, camera);

        return size;
    }

    std::size_t camera_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(camera);

        return size;
    }

    bool camera_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI

        {
            float height = camera.get_height();
            float zNear  = camera.get_zNear();
            float zFar   = camera.get_zFar();

            bool projection_edited = false;

            projection_edited =
                projection_edited || ImGui::DragFloat("Height", &height, 0.1f);
            projection_edited =
                projection_edited || ImGui::DragFloat("zNear", &zNear, 0.1f);
            projection_edited =
                projection_edited || ImGui::DragFloat("zFar", &zFar, 0.1f);

            if (projection_edited)
            {
                edited = true;

                camera.set_projection(height, zNear, zFar);
            }
        }
#endif
        return edited;
    }

} // namespace Kengine