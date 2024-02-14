#pragma once

#include "editor.hxx"

class game_properties_wnd
{
public:
    game_properties_wnd();

    void display();

private:
    bool                  add_new_scene_link = false;
    static constexpr int  max_name_length    = 50;
    std::filesystem::path new_scene_link_path{};
    char                  new_scene_link_name[max_name_length]{};
};
