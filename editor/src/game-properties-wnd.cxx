#include "game-properties-wnd.hxx"

#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/scene/scene.hxx"

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
            auto game_scene_links = current_game->get_scene_links();

            static Kengine::string_id selected_sc_link{};
            static const char*        selected_sc_link_name = "";
            static std::string        selected_sc_link_path = "";
            {
                ImGui::Text("Scene links:");
                ImGui::BeginChild("scene_links",
                                  { 150, 0 },
                                  ImGuiChildFlags_ResizeX |
                                      ImGuiChildFlags_Border);

                for (auto& sc_link : game_scene_links)
                {
                    const auto sc_link_id   = sc_link.first;
                    auto       sc_link_name = Kengine::get_string(sc_link_id);
                    if (ImGui::Selectable(sc_link_name,
                                          sc_link_id == selected_sc_link))
                    {
                        selected_sc_link      = sc_link_id;
                        selected_sc_link_name = sc_link_name;
                        selected_sc_link_path = sc_link.second.string();
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
            if (selected_sc_link && game_scene_links.contains(selected_sc_link))
            {
                auto& sc_link = game_scene_links[selected_sc_link];

                ImGui::BeginGroup();
                ImGui::BeginChild("Scene link info",
                                  { 0, -ImGui::GetFrameHeightWithSpacing() });
                ImGui::Text(selected_sc_link_name);
                ImGui::Separator();
                ImGui::Text(selected_sc_link_path.c_str());
                if (ImGui::Button("Remove"))
                {
                    current_game->remove_scene_link(selected_sc_link);
                    selected_sc_link = 0;
                }
                if (ImGui::Button("Open"))
                {
                    editor::instance->get_current_scene().clear_resources();
                    current_game->set_current_scene(selected_sc_link);
                    editor::instance->invalid_scene_render();
                }
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