#pragma once

#include "Kengine/resources/resource.hxx"
#include "imgui-filebrowser/imfilebrowser.h"

enum class file_type
{
    scene,
    resource
};

class assets_browser
{
public:
    assets_browser();

    void display();

    ImGui::FileBrowser assets_file_browser{
        "Assets",
        ImGuiFileBrowserFlags_Window | ImGuiFileBrowserFlags_CreateNewDir |
            ImGuiFileBrowserFlags_EnterNewFilename
    };

private:
    static constexpr int new_filename_size               = 100;
    char                 new_filename[new_filename_size] = "new_file";

    bool create_resource_modal = false;

    file_type              create_file_type;
    Kengine::resource_type create_res_type;
};