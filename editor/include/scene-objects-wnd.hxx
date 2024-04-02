#pragma once

#include "editor.hxx"

class scene_objects_wnd
{
public:
    scene_objects_wnd();

    void display();

private:
    void draw_entity_entry(Kengine::scene& current_scene, entt::entity);
};
