#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/graphics/camera.hxx"

namespace Kengine
{
    class scene;
    struct component_info;

    struct camera_component : public component
    {
        friend class scene;
        static constexpr auto name = "camera_component";
        static component_info info;

        camera_component();
        camera_component(scene& sc);
        ~camera_component();

        void bind(scene&);

        inline bool is_binded() const { return binded; }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        bool imgui_editable_render() override;

        graphics::camera camera;

    private:
        bool   binded = false;
        scene* cam_scene;
    };

} // namespace Kengine