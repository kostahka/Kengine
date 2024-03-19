#pragma once

#include "imgui-filebrowser/imfilebrowser.h"
#include "Kengine/resources/resource.hxx"

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
    static constexpr int new_filename_size = 100;
    char                 new_filename[new_filename_size];

    bool create_resource_modal = false;

    file_type create_file_type;
    Kengine::resource_type create_res_type;
};