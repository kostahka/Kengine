#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/graphics/camera.hxx"

namespace Kengine
{
    class scene;

    struct camera_component : public component
    {
        friend class scene;

        camera_component();
        camera_component(mat4x4 projection);
        ~camera_component();

        void bind(scene&);

        inline bool is_binded() const { return binded; }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        graphics::camera camera;

    private:
        bool   binded;
        scene* cam_scene;
    };

} // namespace Kengine