#include "Kengine/game.hxx"

#include "../scene/scene-manager.hxx"
#include "Kengine/io/file-manager.hxx"

namespace Kengine
{
    game::game()
        : scene_links()
    {
        current_scene = std::make_shared<scene>();
    }

    void game::set_current_scene(std::shared_ptr<scene> sc)
    {
        if (sc)
            current_scene = sc;
    }

    static path scene_links_path = "./scene-data.kpkg";

    void game::load_scene_links()
    {
        scene_links.clear();

        if (file_manager::file_exists(scene_manager::assets_base_folder /
                                      scene_links_path))
        {
            auto scene_links_file = file_manager::load_file(
                scene_manager::assets_base_folder / scene_links_path);
            if (scene_links_file && scene_links_file->size() > 0)
            {
                std::istream sc_stream(scene_links_file.get());

                auto readed_size = serialization::read(sc_stream, scene_links);
            }
        }
    }

    void game::save_scene_links()
    {
        auto scene_links_file = file_manager::open_file(
            scene_manager::assets_base_folder / scene_links_path,
            std::ios_base::out);
        if (scene_links_file)
        {
            std::ostream sc_stream(scene_links_file.get());

            serialization::write(sc_stream, scene_links);

            sc_stream.flush();
        }

        scene_links_file->close();
    }

    void game::add_scene_link(std::string_view name, std::filesystem::path path)
    {
        auto final_path =
            std::filesystem::relative(path, scene_manager::assets_base_folder);

        scene_links.push_back({ name, final_path });
    }
} // namespace Kengine