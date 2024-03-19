#include "Kengine/components/transform-component.hxx"

#include "imgui.h"

namespace Kengine
{
    transform_component::transform_component()
        : component(name){};

    transform_component::transform_component(const vec3& position)
        : component(name)
        , position(position){};

    std::size_t transform_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, position.x);
        size += serialization::write(os, position.y);
        size += serialization::write(os, angle);
        size += serialization::write(os, scale.x);
        size += serialization::write(os, scale.y);
        size += serialization::write(os, scale.z);

        return size;
    }

    std::size_t transform_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, position.x);
        size += serialization::read(is, position.y);
        size += serialization::read(is, angle);
        size += serialization::read(is, scale.x);
        size += serialization::read(is, scale.y);
        size += serialization::read(is, scale.z);

        return size;
    }

    bool transform_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);
        edited =
            edited || ImGui::DragFloat2("Position", (float*)&position, 0.1f);
        edited = edited || ImGui::DragFloat("Angle", &angle, 0.1f);
        edited = edited || ImGui::DragFloat3("Scale", (float*)&scale, 0.1f);
        ImGui::PopID();
#endif
        return edited;
    }

} // namespace Kengine