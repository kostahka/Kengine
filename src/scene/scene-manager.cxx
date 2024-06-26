#include "scene-manager.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include "../engine.hxx"
#include "Kengine/game.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine::scene_manager
{
    path assets_base_folder{ "/" };

    void set_assets_base_folder(path folder)
    {
        assets_base_folder = folder;
    }

    path get_assets_base_folder()
    {
        return assets_base_folder;
    }

    std::shared_ptr<scene> load_scene(const path& scene_path, game* sc_game)
    {
        auto loaded_scene = std::make_shared<scene>(sc_game);

        auto scene_file =
            file_manager::load_file(assets_base_folder / scene_path);
        if (scene_file && scene_file->size())
        {
            std::istream sc_stream(scene_file.get());

            auto loaded_size = serialization::read(sc_stream, *loaded_scene);
        }

        return loaded_scene;
    }

    void save_scene(std::shared_ptr<scene> sc, const path& scene_path)
    {
        if (sc)
        {
            auto scene_file = file_manager::open_file(
                assets_base_folder / scene_path, std::ios_base::out);
            if (scene_file)
            {
                std::ostream sc_stream(scene_file.get());

                auto loaded_size = serialization::write(sc_stream, *sc);
            }
        }
    }

    void initialize() {}

    void shutdown() {}
} // namespace Kengine::scene_manager