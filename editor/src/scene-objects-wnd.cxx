#include "scene-objects-wnd.hxx"

#include "Kengine/scene/scene.hxx"

#include "imgui.h"

scene_objects_wnd::scene_objects_wnd() {}

void scene_objects_wnd::display()
{
    ImGui::PushID(this);
    auto current_game = editor::instance->current_game;

    {
        ImGui::Begin("Scene objects");

        if (current_game)
        {
            auto current_scene_id = current_game->get_current_scene_id();
            if (current_scene_id)
            {
                Kengine::scene& current_scene =
                    current_game->get_current_scene();

                ImGui::Text("Objects:");
                {
                    ImGui::BeginChild(
                        "objects", { 0, 0 }, ImGuiChildFlags_FrameStyle);

                    auto ent_view = current_scene.registry.view<entt::entity>();

                    for (auto [ent] : ent_view.each())
                    {
                        if (ImGui::Selectable(
                                std::to_string(static_cast<uint32_t>(ent))
                                    .c_str()))
                        {
                            editor::instance->selected_entity = ent;
                        }
                    }

                    ImGui::EndChild();
                    if (ImGui::Button("Add"))
                    {
                        std::ignore = current_scene.registry.create();
                    }
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
