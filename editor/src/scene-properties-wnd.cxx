#include "scene-properties-wnd.hxx"

#include "Kengine/scene/scene.hxx"

#include "imgui.h"

scene_properties_wnd::scene_properties_wnd() {}

void scene_properties_wnd::display()
{
    ImGui::PushID(this);
    auto current_game = editor::instance->current_game;

    {
        ImGui::Begin("Scene properties");

        if (current_game)
        {
            auto current_scene_id = current_game->get_current_scene_id();
            if (current_scene_id)
            {
                Kengine::scene& current_scene =
                    current_game->get_current_scene();
                auto current_scene_name = current_scene_id.get_string();

                auto game_system_factories =
                    current_game->get_system_factories();

                ImGui::Text("Current scene: %s", current_scene_name);
                ImGui::Separator();
                ImGui::Text("Systems:");

                static ImVec2 add_system_popup_pos;
                {
                    ImGui::BeginChild("Systems",
                                      { 0, 0 },
                                      ImGuiChildFlags_FrameStyle |
                                          ImGuiChildFlags_AutoResizeX |
                                          ImGuiChildFlags_AutoResizeY);

                    auto scene_systems = current_scene.get_systems();
                    static Kengine::string_id remove_scene_system{};
                    for (auto& sc_system : scene_systems)
                    {
                        ImGui::PushID(sc_system.first);
                        auto sc_system_name = sc_system.first.get_string();
                        ImGui::BulletText("%s", sc_system_name);
                        ImGui::SameLine();
                        if (ImGui::Button("x"))
                        {
                            remove_scene_system = sc_system.first;
                        }
                        ImGui::PopID();
                    }
                    if (!remove_scene_system.empty())
                    {
                        current_scene.remove_system(remove_scene_system);
                        remove_scene_system = Kengine::string_id();
                    }
                    if (ImGui::Button("Add"))
                    {
                        ImGui::OpenPopup("add_system");
                        add_system_popup_pos = ImGui::GetMousePos();
                    }
                    ImGui::SetNextWindowPos(add_system_popup_pos);
                    if (ImGui::BeginPopup("add_system"))
                    {
                        for (auto& game_system_factory : game_system_factories)
                        {
                            auto system_name =
                                game_system_factory.first.get_string();
                            if (ImGui::Selectable(system_name))
                            {
                                current_scene.add_system(
                                    game_system_factory.first);
                            }
                        }

                        ImGui::EndPopup();
                    }
                    ImGui::EndChild();
                }

                ImGui::Separator();

                ImGui::Text("Scene cameras");
                {
                    ImGui::BeginChild("Cameras",
                                      { 0, 0 },
                                      ImGuiChildFlags_FrameStyle |
                                          ImGuiChildFlags_AutoResizeX |
                                          ImGuiChildFlags_AutoResizeY);

                    auto camera_view = current_scene.registry
                                           .view<Kengine::camera_component>();

                    for (auto [ent, ent_camera] : camera_view.each())
                    {
                        ImGui::PushID(static_cast<uint32_t>(ent));

                        ImGui::Text("Entity %d", static_cast<uint32_t>(ent));
                        if (current_scene.get_camera_entity() != ent)
                        {
                            ImGui::SameLine();
                            if (ImGui::Button("Bind"))
                            {
                                editor::instance->set_scene_camera(ent);
                            }
                        }

                        ImGui::PopID();
                    }

                    ImGui::EndChild();
                }

                ImGui::Text("Resource links:");
                {
                    ImGui::BeginChild("Resources",
                                      { 0, 0 },
                                      ImGuiChildFlags_FrameStyle |
                                          ImGuiChildFlags_AutoResizeX |
                                          ImGuiChildFlags_AutoResizeY);

                    auto scene_resources =
                        editor::instance->get_current_scene().get_resources();
                    for (auto& sc_res_it : scene_resources)
                    {
                        if (sc_res_it.second)
                        {
                            auto res_name = sc_res_it.second->get_resource_id()
                                                .get_string();
                            if (ImGui::Selectable(res_name))
                            {
                                editor::instance->current_res =
                                    sc_res_it.second;
                            }
                        }
                        else
                        {
                            ImGui::BulletText("NULL resource");
                        }
                    }

                    ImGui::EndChild();
                }

                ImGui::Text("Clear color:");
                if (ImGui::ColorEdit4("scene_clear_color",
                                      (float*)&(current_scene.clear_color)))
                {
                    editor::instance->invalid_scene_render();
                }

                if (ImGui::Button("Save scene"))
                {
                    current_game->save_scene(current_scene_id, current_scene);
                }
            }
            else
            {
                ImGui::Text("No opened scene");
            }
        }
        else
        {
            ImGui::Text("No opened game");
        }

        ImGui::End();
    }

    ImGui::PopID();
}
