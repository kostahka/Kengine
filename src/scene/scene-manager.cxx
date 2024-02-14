#include "scene-manager.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include "../engine.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine::scene_manager
{
    std::shared_ptr<scene> current_scene;
    path                   assets_base_folder{ "/" };

    void set_assets_base_folder(path folder)
    {
        assets_base_folder = folder;
    }

    std::shared_ptr<scene> load_scene(const scene_link& scene_link)
    {
        auto loaded_scene = std::make_shared<scene>();

        auto scene_file =
            file_manager::load_file(assets_base_folder / scene_link.get_path());
        if (scene_file && scene_file->size())
        {
            std::istream sc_stream(scene_file.get());

            auto loaded_size = serialization::read(sc_stream, *loaded_scene);
        }

        return loaded_scene;
    }

    void set_current_scene(std::shared_ptr<scene> sc)
    {
        KENGINE_ASSERT(sc, "Do not set NULL scene");
        current_scene = sc;
    }

    std::shared_ptr<scene> get_current_scene()
    {
        KENGINE_ASSERT(current_scene, "Current scene is NULL!");
        return current_scene;
    }

    void initialize()
    {
        current_scene = std::make_shared<scene>();
    }

    void shutdown()
    {
        current_scene.reset();
    }
} // namespace Kengine::scene_manager