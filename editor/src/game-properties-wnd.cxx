#include "game-properties-wnd.hxx"

#include "Kengine/log/log.hxx"

#include "imgui.h"

game_properties_wnd::game_properties_wnd() {}

void game_properties_wnd::display()
{
    ImGui::PushID(this);
    auto current_game = editor::instance->current_game;

    {
        ImGui::Begin("Game properties");
        if (current_game)
        {
            static unsigned int selected_sc_link      = 0;
            static const char*  selected_sc_link_name = "";
            static std::string  selected_sc_link_path = "";
            {
                ImGui::Text("Scene links:");
                ImGui::BeginChild("scene_links",
                                  { 150, 0 },
                                  ImGuiChildFlags_ResizeX |
                                      ImGuiChildFlags_Border);

                for (auto i = 0U; i < current_game->scene_links.size(); ++i)
                {
                    auto& sc_link = current_game->scene_links[i];
                    auto  sc_link_name =
                        Kengine::get_string(sc_link.get_name_id());
                    if (ImGui::Selectable(sc_link_name, i == selected_sc_link))
                    {
                        selected_sc_link      = i;
                        selected_sc_link_name = sc_link_name;
                        selected_sc_link_path =
                            sc_link.get_path().string();
                    }
                }
                ImGui::EndChild();

                if (ImGui::BeginDragDropTarget())
                {
                    if (auto payload =
                            ImGui::AcceptDragDropPayload("FILE_DRAG"))
                    {
                        KENGINE_ASSERT(payload->DataSize ==
                                           sizeof(std::filesystem::path),
                                       "Dropped not path");
                        new_scene_link_path =
                            *reinterpret_cast<std::filesystem::path*>(
                                payload->Data);

                        add_new_scene_link = true;
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            ImGui::SameLine();
            if (selected_sc_link < current_game->scene_links.size())
            {
                auto& sc_link = current_game->scene_links[selected_sc_link];

                ImGui::BeginGroup();
                ImGui::BeginChild("Scene link info",
                                  { 0, -ImGui::GetFrameHeightWithSpacing() });
                ImGui::Text(selected_sc_link_name);
                ImGui::Separator();
                ImGui::Text(selected_sc_link_path.c_str());
                ImGui::EndChild();
                ImGui::EndGroup();
            }
        }
        else
        {
            ImGui::BulletText("No selected game.");
        }
        ImGui::End();
    }

    if (current_game)
    {
        if (add_new_scene_link)
            ImGui::OpenPopup("Add new scene link");

        if (ImGui::BeginPopupModal("Add new scene link", &add_new_scene_link))
        {
            ImGui::InputText("Name", new_scene_link_name, max_name_length);
            if (ImGui::Button("Apply"))
            {
                current_game->add_scene_link(new_scene_link_name,
                                             new_scene_link_path);
                current_game->save_scene_links();
                add_new_scene_link = false;
            }

            ImGui::EndPopup();
        }
    }

    ImGui::PopID();
}