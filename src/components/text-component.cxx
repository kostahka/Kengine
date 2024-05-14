#include "Kengine/components/text-component.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/imgui/imgui-edit.hxx"

#include "imgui.h"

#include <unordered_map>

namespace Kengine
{
    static std::unordered_map<text_align, const char*> text_aligns{
        {text_align::left,    "left"  },
        { text_align::center, "center"},
        { text_align::right,  "right" }
    };

    text_component::text_component()
        : component(name)
        , material(nullptr)
    {
    }

    text_component::text_component(text_component& other)
        : component(name)
        , material(other.material)
        , origin(other.origin)
        , text(other.text)
        , line_advance(other.line_advance)
        , text_size(other.text_size)
        , align(other.align)
        , color(other.color)
        , angle(other.angle)
        , scale(other.scale)
        , layer(other.layer)
        , gui_event_id(other.gui_event_id)
    {
        if (material)
        {
            material->take_data();
        }
    }

    text_component::text_component(text_component&& other)
        : component(name)
        , material(nullptr)
        , origin(other.origin)
        , text(other.text)
        , line_advance(other.line_advance)
        , text_size(other.text_size)
        , align(other.align)
        , color(other.color)
        , angle(other.angle)
        , scale(other.scale)
        , layer(other.layer)
        , gui_event_id(other.gui_event_id)
    {
        std::swap(material, other.material);
    }

    text_component& text_component::operator=(text_component& other)
    {
        if (material)
        {
            material->free_data();
        }
        material     = other.material;
        origin       = other.origin;
        text         = other.text;
        line_advance = other.line_advance;
        text_size    = other.text_size;
        align        = other.align;
        color        = other.color;
        angle        = other.angle;
        scale        = other.scale;
        layer        = other.layer;
        gui_event_id = other.gui_event_id;
        if (material)
        {
            material->take_data();
        }
        return *this;
    }

    text_component& text_component::operator=(text_component&& other)
    {
        std::swap(material, other.material);
        origin       = other.origin;
        text         = other.text;
        line_advance = other.line_advance;
        text_size    = other.text_size;
        align        = other.align;
        color        = other.color;
        angle        = other.angle;
        scale        = other.scale;
        layer        = other.layer;
        gui_event_id = other.gui_event_id;
        return *this;
    }

    void text_component::set_material(
        const res_ptr<font_material_resource>& material)
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

    text_component::~text_component()
    {
        if (material)
        {
            material->free_data();
        }
    }

    std::size_t text_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, text);
        size += serialization::write(os, line_advance);
        size += serialization::write(os, text_size);
        size += serialization::write(os, align);
        size += serialization::write(os, color.r);
        size += serialization::write(os, color.g);
        size += serialization::write(os, color.b);
        size += serialization::write(os, color.a);
        size += serialization::write(os, origin.x);
        size += serialization::write(os, origin.y);
        size += serialization::write(os, angle);
        size += serialization::write(os, scale.x);
        size += serialization::write(os, scale.y);
        size += serialization::write(os, layer);
        size += serialization::write(os, gui_event_id);
        size += serialization::write(os, material);

        return size;
    }

    std::size_t text_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        res_ptr<font_material_resource> material;

        size += serialization::read(is, text);
        size += serialization::read(is, line_advance);
        size += serialization::read(is, text_size);
        size += serialization::read(is, align);
        size += serialization::read(is, color.r);
        size += serialization::read(is, color.g);
        size += serialization::read(is, color.b);
        size += serialization::read(is, color.a);
        size += serialization::read(is, origin.x);
        size += serialization::read(is, origin.y);
        size += serialization::read(is, angle);
        size += serialization::read(is, scale.x);
        size += serialization::read(is, scale.y);
        size += serialization::read(is, layer);
        size += serialization::read(is, gui_event_id);
        size += serialization::read(is, material);

        set_material(material);

        return size;
    }

    std::size_t text_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(text);
        size += serialization::size(line_advance);
        size += serialization::size(text_size);
        size += serialization::size(align);
        size += serialization::size(color.r);
        size += serialization::size(color.g);
        size += serialization::size(color.b);
        size += serialization::size(color.a);
        size += serialization::size(origin.x);
        size += serialization::size(origin.y);
        size += serialization::size(angle);
        size += serialization::size(scale.x);
        size += serialization::size(scale.y);
        size += serialization::size(layer);
        size += serialization::size(gui_event_id);
        size += serialization::size(material);

        return size;
    }

    bool text_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        auto new_material = material;
        if (imgui::edit_resource("Font material", &new_material))
        {
            set_material(new_material);
            edited = true;
        }

        static std::vector<char32_t> input_text{
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };
        if (ImGui::InputTextMultiline(
                "Text", (char*)input_text.data(), input_text.size()))
        {
            text = input_text.data();
            if (input_text.size() < text.size() - 10)
            {
                input_text.resize(input_text.size() + 10);
            }
            edited = true;
        }
        edited =
            edited || ImGui::DragFloat("Text size", (float*)&text_size, 0.1f);
        edited =
            edited || ImGui::DragFloat("Line advance", &line_advance, 0.1f);

        auto cur_align = text_aligns.find(align);
        if (ImGui::BeginCombo("Align", cur_align->second))
        {
            for (auto& align_it : text_aligns)
            {
                if (ImGui::Selectable(align_it.second))
                {
                    align  = align_it.first;
                    edited = true;
                }
            }
            ImGui::EndCombo();
        }

        edited = edited || ImGui::ColorEdit4("Color", (float*)&(color));
        edited = edited || ImGui::DragFloat2("Origin", (float*)&origin, 0.1f);
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