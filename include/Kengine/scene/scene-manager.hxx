#pragma once

#include <memory>

namespace Kengine
{
    class game;
}

namespace Kengine::scene_manager
{
    E_DECLSPEC std::shared_ptr<scene> load_scene(const path& scene_path,
                                                 game*       sc_game);
    E_DECLSPEC void                   save_scene(std::shared_ptr<scene> sc,
                                                 const path&            scene_path);
    E_DECLSPEC void                   set_assets_base_folder(path);
    E_DECLSPEC path                   get_assets_base_folder();
} // namespace Kengine::scene_manager
