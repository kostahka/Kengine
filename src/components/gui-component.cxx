#include "Kengine/components/gui-component.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/imgui/imgui-edit.hxx"

#include "imgui.h"

namespace Kengine
{
    gui_component::gui_component()
        : component(name)
        , material(nullptr)
    {
    }

    gui_component::gui_component(gui_component& other)
        : component(name)
        , material(other.material)
        , uv(other.uv)
        , origin(other.origin)
        , angle(other.angle)
        , scale(other.scale)
        , layer(other.layer)
        , gui_event_id(other.gui_event_id)
    {
        material->take_data();
    }

    gui_component::gui_component(gui_component&& other)
        : component(name)
        , material(nullptr)
        , uv(other.uv)
        , origin(other.origin)
        , angle(other.angle)
        , scale(other.scale)
        , layer(other.layer)
        , gui_event_id(other.gui_event_id)
    {
        std::swap(material, other.material);
    }

    gui_component& gui_component::operator=(gui_component& other)
    {
        uv           = other.uv;
        origin       = other.origin;
        material     = other.material;
        angle        = other.angle;
        scale        = other.scale;
        layer        = other.layer;
        gui_event_id = other.gui_event_id;
        material->take_data();
        return *this;
    }

    gui_component& gui_component::operator=(gui_component&& other)
    {
        uv     = other.uv;
        origin = other.origin;
        std::swap(material, other.material);
        angle        = other.angle;
        scale        = other.scale;
        layer        = other.layer;
        gui_event_id = other.gui_event_id;
        return *this;
    }

    void gui_component::set_material(
        const res_ptr<gui_material_resource>& material)
    {
        if (this->material)
        {
            this->material->free_data();
        }
        this->material = material;
        if (this->material)
        {
            this->material->take_data();
        }
    }

    gui_component::~gui_component()
    {
        if (material)
            material->free_data();
    }

    std::size_t gui_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, origin.x);
        size += serialization::write(os, origin.y);
        size += serialization::write(os, uv.x);
        size += serialization::write(os, uv.y);
        size += serialization::write(os, uv.w);
        size += serialization::write(os, uv.h);
        size += serialization::write(os, angle);
        size += serialization::write(os, scale.x);
        size += serialization::write(os, scale.y);
        size += serialization::write(os, layer);
        size += serialization::write(os, material);
        size += serialization::write(os, gui_event_id);

        return size;
    }

    std::size_t gui_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        res_ptr<gui_material_resource> material;

        size += serialization::read(is, origin.x);
        size += serialization::read(is, origin.y);
        size += serialization::read(is, uv.x);
        size += serialization::read(is, uv.y);
        size += serialization::read(is, uv.w);
        size += serialization::read(is, uv.h);
        size += serialization::read(is, angle);
        size += serialization::read(is, scale.x);
        size += serialization::read(is, scale.y);
        size += serialization::read(is, layer);
        size += serialization::read(is, material);
        size += serialization::read(is, gui_event_id);

        set_material(material);

        return size;
    }

    std::size_t gui_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(origin.x);
        size += serialization::size(origin.y);
        size += serialization::size(uv.x);
        size += serialization::size(uv.y);
        size += serialization::size(uv.w);
        size += serialization::size(uv.h);
        size += serialization::size(angle);
        size += serialization::size(scale.x);
        size += serialization::size(scale.y);
        size += serialization::size(layer);
        size += serialization::size(material);
        size += serialization::size(gui_event_id);

        return size;
    }

    bool gui_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        auto new_material = material;
        if (imgui::edit_resource("Sprite material", &new_material))
        {
            set_material(new_material);
            edited = true;
        }

        edited = edited || ImGui::DragFloat2("Origin", (float*)&origin, 0.1f);
        edited = edited || ImGui::DragFloat4("UV", (float*)&uv, 0.1f);
        edited = edited || ImGui::DragFloat("Angle", (float*)&angle, 0.1f);
        edited = edited || ImGui::DragFloat2("Scale", (float*)&scale, 0.1f);
        edited = edited || ImGui::DragInt("Layer", &layer);

        ImGui::BulletText("Gui event id: '%s'", gui_event_id.get_string());
        static const int gui_event_name_length                 = 100;
        static char      gui_event_name[gui_event_name_length] = "";
        ImGui::InputText(
            "New gui event id", gui_event_name, gui_event_name_length);
        ImGui::SameLine();
        if (ImGui::Button("Set id"))
        {
            gui_event_id = hash_string(gui_event_name);
        }

        ImGui::PopID();
#endif
        return edited;
    }

} // namespace Kengine