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

    camera_component::camera_component(scene& sc)
        : component(name)
        , camera()
        , cam_scene(&sc)
    {
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

    bool camera_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        if (ImGui::Button("Bind"))
        {
            bind(*cam_scene);
            edited = true;
        }
        ImGui::Text("Binded: %s", binded ? "true" : "false");

        {
            float width  = camera.get_width();
            float height = camera.get_height();
            float zNear  = camera.get_zNear();
            float zFar   = camera.get_zFar();

            bool projection_edited = false;

            projection_edited =
                projection_edited || ImGui::DragFloat("Width", &width, 0.1f);
            projection_edited =
                projection_edited || ImGui::DragFloat("Height", &height, 0.1f);
            projection_edited =
                projection_edited || ImGui::DragFloat("zNear", &zNear, 0.1f);
            projection_edited =
                projection_edited || ImGui::DragFloat("zFar", &zFar, 0.1f);

            if (projection_edited)
            {
                edited = true;

                camera.set_projection(width, height, zNear, zFar);
            }
        }
#endif
        return edited;
    }

    component_info camera_component::info{
        [](scene& sc, entt::entity ent)
        { return sc.registry.any_of<camera_component>(ent); },

        [](scene& sc, entt::entity ent) {
            return static_cast<component*>(
                &sc.registry.get<camera_component>(ent));
        },

        [](entt::snapshot& snapshot, archive_output& output)
        { snapshot.get<camera_component>(output); },

        [](entt::snapshot_loader& snapshot, archive_input& input)
        { snapshot.get<camera_component>(input); },

        [](scene& sc, entt::entity ent)
        { sc.registry.erase<camera_component>(ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.emplace<camera_component>(ent, sc); },

        [](scene& sc, entt::entity ent)
        { sc.registry.patch<camera_component>(ent); },
    };
} // namespace Kengine