#pragma once

#include "Kengine/scene/scene.hxx"

#include <memory>

namespace Kengine::scene_manager
{
    extern path assets_base_folder;

    void initialize();
    void shutdown();
} // namespace Kengine::scene_manager