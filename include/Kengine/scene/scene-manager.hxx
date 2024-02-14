#pragma once

#include "Kengine/scene/scene-link.hxx"

#include <memory>

namespace Kengine::scene_manager
{
    std::shared_ptr<scene> load_scene(const scene_link& scene_link);
    void                   set_assets_base_folder(path);
} // namespace Kengine::scene_manager
