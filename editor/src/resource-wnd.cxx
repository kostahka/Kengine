#include "resource-wnd.hxx"

#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/resource-manager.hxx"

#include "imgui.h"

resource_wnd::resource_wnd() {}

void resource_wnd::display()
{
    ImGui::PushID(this);

    auto current_res = editor::instance->current_res;

    {
        ImGui::Begin("Resource");

        if (current_res)
        {
            auto current_res_name = current_res->get_resource_id().get_string();
            auto current_res_type = current_res->get_resource_type();

            ImGui::Text("Resource name: '%s'; type: '%s'",
                        current_res_name,
                        Kengine::get_resource_type_str(current_res_type));

            {
                ImGui::BeginChild("Resource edit",
                                  { 50, 50 },
                                  ImGuiChildFlags_ResizeX |
                                      ImGuiChildFlags_ResizeY |
                                      ImGuiChildFlags_FrameStyle);

                if (current_res->imgui_editable_render())
                {
                    current_res->reload_data();
                    editor::instance->invalid_scene_render();
                }

                ImGui::EndChild();
            }

            if (current_res_type == Kengine::resource_type::font)
            {
                ImGui::BeginChild("Prefiew",
                                  { 50, 50 },
                                  ImGuiChildFlags_ResizeX |
                                      ImGuiChildFlags_ResizeY |
                                      ImGuiChildFlags_FrameStyle);

                static Kengine::res_ptr<Kengine::font_resource>
                    display_font_res = nullptr;

                auto font_res =
                    Kengine::static_resource_cast<Kengine::font_resource>(
                        current_res);

                if (display_font_res != font_res)
                {
                    if (display_font_res)
                    {
                        display_font_res->free_data();
                    }
                    display_font_res = font_res;
                    if (display_font_res)
                    {
                        display_font_res->take_data();
                    }
                }

                if (display_font_res)
                {
                    auto image_size = display_font_res->get_texture_size();
                    ImGui::Image(
                        (ImTextureID)display_font_res->get_id(),
                        { (float)image_size.x * 2, (float)image_size.y * 2 });
                }

                ImGui::EndChild();
            }

            if (ImGui::Button("Save"))
            {
                Kengine::resource_manager::save_resource(current_res);
            }
        }
        else
        {
            ImGui::Text("No current resource");
        }

        ImGui::End();
    }

    ImGui::PopID();
}