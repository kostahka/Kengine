#pragma once

#include <memory>

namespace Kengine
{
    class game;
}

namespace Kengine::scene_manager
{
    std::shared_ptr<scene> load_scene(const path& scene_path, game* sc_game);
    void                   set_assets_base_folder(path);
} // namespace Kengine::scene_manager
