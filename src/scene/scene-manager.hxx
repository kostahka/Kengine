#pragma once

#include "Kengine/scene/scene.hxx"

namespace Kengine::scene_manager
{
    extern scene* current_scene;

    void set_current_scene(scene*);

    void initialize();
    void shutdown();
} // namespace Kengine::scene_manager