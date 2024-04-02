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

        camera_component();
        ~camera_component();

        camera_component(camera_component& other);
        camera_component(camera_component&& other);

        camera_component& operator=(camera_component& other);
        camera_component& operator=(camera_component&& other);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        graphics::camera camera;
    };

} // namespace Kengine