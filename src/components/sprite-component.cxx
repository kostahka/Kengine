#include "Kengine/components/sprite-component.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/imgui/imgui-edit.hxx"

#include "imgui.h"

namespace Kengine
{
    sprite_component::sprite_component()
        : component(name)
        , material(nullptr)
    {
    }

    sprite_component::sprite_component(
        const res_ptr<sprite_material_resource>& material, rect uv, vec2 origin)
        : component(name)
        , material(material)
        , uv(uv)
        , origin(origin)
    {
        this->material->take_data();
    }

    sprite_component::sprite_component(sprite_component&& other)
        : sprite_component()
    {
        std::swap(material, other.material);
        origin = other.origin;
        uv     = other.uv;
    }

    sprite_component& sprite_component::operator=(sprite_component&& other)
    {
        std::swap(material, other.material);
        origin = other.origin;
        uv     = other.uv;
        return *this;
    }

    void sprite_component::set_material(
        const res_ptr<sprite_material_resource>& material)
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

    sprite_component::~sprite_component()
    {
        if (material)
            material->free_data();
    }

    std::size_t sprite_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, origin.x);
        size += serialization::write(os, origin.y);
        size += serialization::write(os, uv.x);
        size += serialization::write(os, uv.y);
        size += serialization::write(os, uv.w);
        size += serialization::write(os, uv.h);
        size += serialization::write(os, material);

        return size;
    }

    std::size_t sprite_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        res_ptr<sprite_material_resource> material;

        size += serialization::read(is, origin.x);
        size += serialization::read(is, origin.y);
        size += serialization::read(is, uv.x);
        size += serialization::read(is, uv.y);
        size += serialization::read(is, uv.w);
        size += serialization::read(is, uv.h);
        size += serialization::read(is, material);

        set_material(material);

        return size;
    }

    bool sprite_component::imgui_editable_render()
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

        ImGui::PopID();
#endif

        return edited;
    }

} // namespace Kengine