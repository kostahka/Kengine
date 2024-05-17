#include "scene-objects-wnd.hxx"

#include "Kengine/components/rect-transform-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

static std::unordered_map<entt::entity, std::vector<entt::entity>>
                    entities_tree;
static entt::entity delete_entity = entt::null;

scene_objects_wnd::scene_objects_wnd() {}

void scene_objects_wnd::draw_entity_entry(Kengine::scene& current_scene,
                                          entt::entity    ent)
{

    static ImGuiTreeNodeFlags base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

    if (ImGui::Button("Delete"))
    {
        delete_entity = ent;
    }
    ImGui::SameLine();

    bool entity_open = ImGui::TreeNodeEx(
        (void*)ent, base_flags, "Entity %d", static_cast<uint32_t>(ent));
    if (ImGui::IsItemHovered() &&
        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) &&
        !ImGui::IsItemToggledOpen())
    {
        editor::instance->selected_entity = ent;
    }
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("ENTITY_DRAG", &ent, sizeof(entt::entity));
        ImGui::Text("Entity %d", static_cast<uint32_t>(ent));
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (auto payload = ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
        {
            KENGINE_ASSERT(payload->DataSize == sizeof(entt::entity),
                           "Dropped not entity");
            auto drag_entity = *reinterpret_cast<entt::entity*>(payload->Data);

            if (ent != drag_entity)
            {
                bool is_ent_transform =
                    current_scene.registry.any_of<Kengine::transform_component>(
                        ent);
                bool is_drag_ent_transform =
                    current_scene.registry.any_of<Kengine::transform_component>(
                        drag_entity);
                bool is_ent_rect_transform =
                    current_scene.registry
                        .any_of<Kengine::rect_transform_component>(ent);
                bool is_drag_ent_rect_transform =
                    current_scene.registry
                        .any_of<Kengine::rect_transform_component>(drag_entity);

                if (is_ent_transform && is_drag_ent_transform)
                {
                    auto& ent_transform =
                        current_scene.registry
                            .get<Kengine::transform_component>(drag_entity);

                    ent_transform.set_parent(ent);
                }
                if (is_ent_rect_transform && is_drag_ent_rect_transform)
                {
                    auto& ent_transform =
                        current_scene.registry
                            .get<Kengine::rect_transform_component>(
                                drag_entity);

                    ent_transform.set_parent(ent);
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (entity_open)
    {
        auto entity_childs = entities_tree.find(ent);
        if (entity_childs != entities_tree.end())
        {
            for (auto child : entity_childs->second)
            {
                draw_entity_entry(current_scene, child);
            }
        }
        ImGui::TreePop();
    }
}

void scene_objects_wnd::display()
{
    ImGui::PushID(this);
    auto current_game = editor::instance->current_game;

    {
        if (ImGui::Begin("Scene objects"))
        {
            if (current_game)
            {
                auto current_scene_id = current_game->get_current_scene_id();
                if (current_scene_id)
                {
                    Kengine::scene& current_scene =
                        current_game->get_current_scene();

                    ImGui::Text("Objects:");
                    {

                        ImGui::BeginChild("objects",
                                          { 0, 0 },
                                          ImGuiChildFlags_FrameStyle |
                                              ImGuiChildFlags_ResizeX |
                                              ImGuiChildFlags_ResizeY);

                        auto ent_view =
                            current_scene.registry.view<entt::entity>();

                        for (auto& ent_childs : entities_tree)
                        {
                            ent_childs.second.clear();
                        }
                        for (auto [ent] : ent_view.each())
                        {
                            if (current_scene.registry
                                    .any_of<Kengine::transform_component>(ent))
                            {
                                auto& ent_transform =
                                    current_scene.registry
                                        .get<Kengine::transform_component>(ent);

                                auto parent = ent_transform.get_parent();
                                if (!current_scene.registry.valid(parent))
                                {
                                    ent_transform.set_parent(entt::null);
                                }
                                if (parent != entt::null)
                                {
                                    entities_tree[parent].push_back(ent);
                                }
                            }
                            else if (current_scene.registry.any_of<
                                         Kengine::rect_transform_component>(
                                         ent))
                            {
                                auto& ent_transform =
                                    current_scene.registry
                                        .get<Kengine::rect_transform_component>(
                                            ent);

                                auto parent = ent_transform.get_parent();
                                if (!current_scene.registry.valid(parent))
                                {
                                    ent_transform.set_parent(entt::null);
                                }
                                if (parent != entt::null)
                                {
                                    entities_tree[parent].push_back(ent);
                                }
                            }
                        }

                        for (auto [ent] : ent_view.each())
                        {
                            ImGui::PushID(static_cast<uint32_t>(ent));

                            if (current_scene.registry
                                    .any_of<Kengine::transform_component>(ent))
                            {
                                auto& ent_transform =
                                    current_scene.registry
                                        .get<Kengine::transform_component>(ent);

                                auto parent = ent_transform.get_parent();
                                if (parent == entt::null)
                                {
                                    draw_entity_entry(current_scene, ent);
                                }
                            }
                            else if (current_scene.registry.any_of<
                                         Kengine::rect_transform_component>(
                                         ent))
                            {
                                auto& ent_transform =
                                    current_scene.registry
                                        .get<Kengine::rect_transform_component>(
                                            ent);

                                auto parent = ent_transform.get_parent();
                                if (parent == entt::null)
                                {
                                    draw_entity_entry(current_scene, ent);
                                }
                            }
                            else
                            {
                                draw_entity_entry(current_scene, ent);
                            }

                            ImGui::PopID();
                        }

                        ImGui::EndChild();
                        if (ImGui::BeginDragDropTarget())
                        {
                            if (auto payload =
                                    ImGui::AcceptDragDropPayload("FILE_DRAG"))
                            {
                                KENGINE_ASSERT(
                                    payload->DataSize ==
                                        sizeof(std::filesystem::path),
                                    "Dropped not path");
                                auto prefab_path =
                                    *reinterpret_cast<std::filesystem::path*>(
                                        payload->Data);

                                if (prefab_path.extension() == ".ksc")
                                {
                                    auto loaded_prefab =
                                        Kengine::scene_manager::load_scene(
                                            prefab_path, current_game);
                                    if (loaded_prefab)
                                    {
                                        current_scene.instansiate(
                                            loaded_prefab);
                                    }
                                }
                            }
                            if (auto payload =
                                    ImGui::AcceptDragDropPayload("ENTITY_DRAG"))
                            {
                                KENGINE_ASSERT(payload->DataSize ==
                                                   sizeof(entt::entity),
                                               "Dropped not entity");
                                auto drag_entity =
                                    *reinterpret_cast<entt::entity*>(
                                        payload->Data);

                                bool is_drag_ent_transform =
                                    current_scene.registry
                                        .any_of<Kengine::transform_component>(
                                            drag_entity);
                                bool is_drag_ent_rect_transform =
                                    current_scene.registry.any_of<
                                        Kengine::rect_transform_component>(
                                        drag_entity);

                                if (is_drag_ent_transform)
                                {
                                    auto& ent_transform =
                                        current_scene.registry
                                            .get<Kengine::transform_component>(
                                                drag_entity);

                                    ent_transform.set_parent(entt::null);
                                }
                                if (is_drag_ent_rect_transform)
                                {
                                    auto& ent_transform =
                                        current_scene.registry.get<
                                            Kengine::rect_transform_component>(
                                            drag_entity);

                                    ent_transform.set_parent(entt::null);
                                }
                            }
                            ImGui::EndDragDropTarget();
                        }
                        if (delete_entity != entt::null)
                        {
                            current_scene.registry.destroy(delete_entity);
                            delete_entity = entt::null;
                            editor::instance->invalid_scene_render();
                        }
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
        }
        ImGui::End();
    }

    ImGui::PopID();
}
