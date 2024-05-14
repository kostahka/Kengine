#include "Kengine/resources/sprite-material-resource.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/resources/resource-manager.hxx"

#include "imgui.h"

namespace Kengine
{
    sprite_material_resource::sprite_material_resource(std::string_view name)
        : material_resource(graphics::sprite_shader, name)
    {
        KENGINE_INFO("Take sprite_shader");
        resource::r_type = resource_type::sprite_material;
    }

    sprite_material_resource::sprite_material_resource(
        const res_ptr<fragment_shader_res>& fragment_shader,
        std::string_view                    shader_name,
        std::string_view                    name)
        : material_resource(
              make_resource<shader_res>(
                  graphics::sprite_vertex_shader, fragment_shader, shader_name),
              name)
    {
    }

    sprite_material_resource::~sprite_material_resource() {}

    std::size_t sprite_material_resource::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, custom_fragment_shader);
        size += serialization::write(os, custom_shader_name);
        if (!custom_fragment_shader)
        {
            size += serialization::write(os, shader);
        }
        size += serialization::write(os, properties);
        size += serialization::write(os, textures);

        return size;
    }

    std::size_t sprite_material_resource::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, custom_fragment_shader);
        size += serialization::read(is, custom_shader_name);
        if (!custom_fragment_shader)
        {
            size += serialization::read(is, shader);
        }
        else
        {
            shader = make_resource<shader_res>(
                graphics::sprite_vertex_shader,
                custom_fragment_shader,
                std::string_view(custom_shader_name.get_string()));
        }
        size += serialization::read(is, properties);
        size += serialization::read(is, textures);

        return size;
    }

    std::size_t sprite_material_resource::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(custom_fragment_shader);
        size += serialization::size(custom_shader_name);
        if (!custom_fragment_shader)
        {
            size += serialization::size(shader);
        }
        size += serialization::size(properties);
        size += serialization::size(textures);

        return size;
    }

    bool sprite_material_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::Text("Shader: %s",
                    shader ? shader->get_resource_id().get_string() : "NONE");

        static res_ptr<fragment_shader_res> custom_fragment_shader = nullptr;
        if (imgui::edit_resource("Custom fragment shader",
                                 &custom_fragment_shader))
        {
        }
        static constexpr int custom_shader_name_length = 100;
        static char          custom_shader_name[custom_shader_name_length];
        ImGui::InputText("New custom shader name",
                         custom_shader_name,
                         custom_shader_name_length);
        if (ImGui::Button("Set new shader"))
        {
            this->custom_fragment_shader = custom_fragment_shader;
            this->custom_shader_name     = hash_string(custom_shader_name);
            res_ptr<shader_res> custom_shader =
                make_resource<shader_res>(graphics::sprite_vertex_shader,
                                          custom_fragment_shader,
                                          std::string_view(custom_shader_name));
            if (p_count > 0)
            {
                if (shader)
                {
                    shader->free_data();
                }
            }
            shader = custom_shader;
            if (p_count > 0)
            {
                if (shader)
                {
                    shader->take_data();
                }
            }
        }

        ImGui::PushID(this);
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