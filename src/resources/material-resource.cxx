#include "Kengine/resources/material-resource.hxx"

#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/log/log.hxx"

#include "imgui.h"

namespace Kengine
{
    material_resource::material_resource(std::string_view name)
        : resource(resource_type::material, name)
        , shader(nullptr)
    {
    }

    material_resource::material_resource(const res_ptr<shader_res>& shader,
                                         std::string_view           name)
        : resource(resource_type::material, name)
        , shader(shader)
    {
    }

    material_resource::~material_resource()
    {
        if (p_count > 0)
        {
            this->unload_data();
        }
    }

    void material_resource::set_texture(
        uint32_t texture_block, const res_ptr<texture_resource>& texture)
    {
        KENGINE_ASSERT(texture_block < 32,
                       "Texture block id must be less than 32");
        textures[texture_block] = texture;
        if (p_count > 0)
        {
            textures[texture_block]->take_data();
        }
    }

    void material_resource::set_property(string_id                 name,
                                         graphics::shader_type_any value)
    {
        properties[name] = value;
    }

    graphics::shader_type_any material_resource::get_property(
        string_id name) const
    {
        auto property = properties.find(name);
        if (property != properties.end())
            return property->second;

        return graphics::shader_type_any();
    }

    std::size_t material_resource::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, shader);
        size += serialization::write(os, properties);
        size += serialization::write(os, textures);

        return size;
    }

    std::size_t material_resource::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, shader);
        size += serialization::read(is, properties);
        size += serialization::read(is, textures);

        return size;
    }

    std::size_t material_resource::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(shader);
        size += serialization::size(properties);
        size += serialization::size(textures);

        return size;
    }

    void material_resource::load_data()
    {
        shader->take_data();
        for (auto& [key, texture] : textures)
        {
            texture->take_data();
        }
    }

    void material_resource::unload_data()
    {
        shader->free_data();
        for (auto& [key, texture] : textures)
        {
            texture->free_data();
        }
    }

    bool material_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        edited = edited || imgui::edit_resource("Shader", &shader);

        {
            ImGui::BeginChild("Textures",
                              { 100, 100 },
                              ImGuiChildFlags_ResizeX |
                                  ImGuiChildFlags_ResizeY |
                                  ImGuiChildFlags_Border);

            for (auto& tex : textures)
            {
                ImGui::Text("%s", tex.second->get_resource_id().get_string());
            }

            ImGui::EndChild();
        }

        static int                       add_texture_id = 0;
        static res_ptr<texture_resource> add_texture    = nullptr;

        ImGui::InputInt("Texture id", &add_texture_id);
        imgui::edit_resource("Texture to add", &add_texture);

        if (ImGui::Button("Add texture") && add_texture && add_texture_id >= 0)
        {
            textures[add_texture_id] = add_texture;
            add_texture              = nullptr;
            edited                   = true;
        }

        ImGui::PopID();
#endif
        return edited;
    }
} // namespace Kengine