#include "object-properties-wnd.hxx"

#include "Kengine/scene/scene.hxx"

#include "imgui.h"

object_properties_wnd::object_properties_wnd() {}

void object_properties_wnd::display()
{
    ImGui::PushID(this);
    auto current_game = editor::instance->current_game;

    {
        ImGui::Begin(window_name);

        if (current_game)
        {
            auto current_scene_id = current_game->get_current_scene_id();
            if (current_scene_id)
            {
                Kengine::scene& current_scene =
                    current_game->get_current_scene();

                auto selected_entity = editor::instance->selected_entity;
                if (current_scene.registry.valid(selected_entity))
                {
                    ImGui::Text("Selected object: %d",
                                static_cast<int>(selected_entity));

                    auto game_component_infos =
                        current_game->get_component_infos();

                    {
                        ImGui::BeginChild("Components",
                                          { 0, 0 },
                                          ImGuiChildFlags_FrameStyle |
                                              ImGuiChildFlags_AutoResizeX |
                                              ImGuiChildFlags_AutoResizeY);

                        Kengine::erase_component_fp erase_component = nullptr;
                        for (auto& c_info_it : game_component_infos)
                        {
                            if (c_info_it.second.has_component(current_scene,
                                                               selected_entity))
                            {
                                auto component_name =
                                    c_info_it.first.get_string();

                                {
                                    ImGui::BeginChild(
                                        component_name,
                                        { 0, 0 },
                                        ImGuiChildFlags_Border |
                                            ImGuiChildFlags_AutoResizeX |
                                            ImGuiChildFlags_AutoResizeY);

                                    ImGui::SeparatorText(component_name);
                                    ImGui::SameLine();
                                    if (ImGui::Button("Remove"))
                                    {
                                        erase_component =
                                            c_info_it.second.erase_component;
                                    }

                                    if (c_info_it.second
                                            .get_component(current_scene,
                                                           selected_entity)
                                            ->imgui_editable_render())
                                    {
                                        c_info_it.second.patch_component(
                                            current_scene, selected_entity);
                                        editor::instance
                                            ->invalid_scene_render();
                                    }

                                    ImGui::EndChild();
                                }
                            }
                        }
                        if (erase_component)
                        {
                            erase_component(current_scene, selected_entity);
                            editor::instance->invalid_scene_render();
                            erase_component = nullptr;
                        }

                        ImGui::EndChild();
                    }

                    static ImVec2 add_component_pos;

                    if (ImGui::Button("Add component"))
                    {
                        add_component_pos = ImGui::GetMousePos();
                        ImGui::OpenPopup("add_component");
                    }

                    ImGui::SetNextWindowPos(add_component_pos);
                    if (ImGui::BeginPopup("add_component"))
                    {
                        for (auto& c_info_it : game_component_infos)
                        {
                            if (!c_info_it.second.has_component(
                                    current_scene, selected_entity))
                            {
                                auto component_name =
                                    c_info_it.first.get_string();

                                if (ImGui::Selectable(component_name))
                                {
                                    c_info_it.second.emplace_component(
                                        current_scene, selected_entity);
                                    editor::instance->invalid_scene_render();
                                }
                            }
                        }
                        ImGui::EndPopup();
                    }
                }
                else
                {
                    ImGui::Text("No selected object");
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
