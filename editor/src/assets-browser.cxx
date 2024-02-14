#include "assets-browser.hxx"

#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene.hxx"

#include <memory>

assets_browser::assets_browser() {}

void assets_browser::display()
{
    ImGui::PushID(this);
    assets_file_browser.Display();

    {
        static ImVec2 create_asset_popup_pos;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right) &&
            assets_file_browser.IsHovered())
        {
            ImGui::OpenPopup("create_asset");
            create_asset_popup_pos = ImGui::GetMousePos();
        }

        ImGui::SetNextWindowPos(create_asset_popup_pos);
        if (ImGui::BeginPopup("create_asset"))
        {
            if (ImGui::Selectable("Create scene"))
            {
                create_resource_modal = true;
                create_file_type      = file_type::scene;
            }
            ImGui::EndPopup();
        }

        if (create_resource_modal)
        {
            ImGui::OpenPopup("create_resource");
            ImGui::SetNextWindowSize({ 400, 100 });
        }

        if (ImGui::BeginPopupModal("create_resource",
                                   &create_resource_modal,
                                   ImGuiWindowFlags_NoResize))
        {
            ImGui::InputText("Filename:", new_filename, new_filename_size);
            if (ImGui::Button("Apply"))
            {
                create_resource_modal = false;
                std::filesystem::path new_file_path =
                    assets_file_browser.GetPwd();

                std::string filename{ new_filename };

                if (create_file_type == file_type::scene)
                {
                    new_file_path  = new_file_path / (filename + ".ksc");
                    auto new_scene = std::make_shared<Kengine::scene>();
                    auto new_file  = Kengine::file_manager::open_file(
                        new_file_path, std::ios_base::out);
                    if (new_file)
                    {
                        std::ostream ofile{ new_file.get() };
                        Kengine::serialization::write(ofile, *new_scene);
                        assets_file_browser.UpdateFileRecords();
                    }
                }
                else if (create_file_type == file_type::resource)
                {
                }
            }
            ImGui::EndPopup();
        }
    }

    ImGui::PopID();
}
