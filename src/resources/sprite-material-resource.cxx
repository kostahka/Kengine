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

    bool sprite_material_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
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