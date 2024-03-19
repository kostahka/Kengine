#include "Kengine/imgui/imgui-edit.hxx"

#include "Kengine/log/log.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "imgui.h"

namespace Kengine::imgui
{
    bool edit_resource(const char* name, res_ptr<resource>* res)
    {
        bool changed = false;
        {
            ImGui::Text("%s:", name);
            ImGui::SameLine();
            ImGui::BeginChild(name,
                              { 0, 0 },
                              ImGuiChildFlags_Border |
                                  ImGuiChildFlags_AutoResizeX |
                                  ImGuiChildFlags_AutoResizeY);

            if (res && (*res))
            {
                ImGui::Text("%s", (*res)->get_resource_id().get_string());
            }
            else
            {
                ImGui::Text("NULL resource");
            }
            ImGui::EndChild();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (auto payload = ImGui::AcceptDragDropPayload("RESOURCE_DRAG"))
            {
                KENGINE_ASSERT(payload->DataSize == sizeof(res_ptr<resource>),
                               "Dropped not res_ptr<resource>");
                *res    = *reinterpret_cast<res_ptr<resource>*>(payload->Data);
                changed = true;
            }
            if (auto payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
            {
                KENGINE_ASSERT(payload->DataSize == sizeof(path),
                               "Dropped not std::filesystem::path");
                auto r_path = *reinterpret_cast<path*>(payload->Data);

                *res = resource_manager::load_resource(r_path);

                changed = true;
            }
            ImGui::EndDragDropTarget();
        }

        return changed;
    }

    bool edit_file(const char* name, std::filesystem::path& path)
    {
        bool changed = false;

        ImGui::Text("%s:", name);
        ImGui::SameLine();
        {
            ImGui::BeginChild(name,
                              { 0, 0 },
                              ImGuiChildFlags_Border |
                                  ImGuiChildFlags_AutoResizeX |
                                  ImGuiChildFlags_AutoResizeY);

            ImGui::Text("%s", path.string().c_str());

            ImGui::EndChild();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (auto payload = ImGui::AcceptDragDropPayload("FILE_DRAG"))
            {
                KENGINE_ASSERT(payload->DataSize == sizeof(path),
                               "Dropped not std::filesystem::path");
                auto f_path =
                    *reinterpret_cast<std::filesystem::path*>(payload->Data);
                path = resource_manager::relative_assets_path(f_path);

                changed = true;
            }
            ImGui::EndDragDropTarget();
        }

        return changed;
    }
} // namespace Kengine::imgui