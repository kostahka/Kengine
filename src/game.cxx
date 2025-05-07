#include "Kengine/game.hxx"

#include "../scene/scene-manager.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine
{
    game::game()
    {
        current_scene = std::make_shared<scene>(this);
        current_scene->set_game(this);
        current_scene_id = 0;
    }

    void game::set_current_scene(string_id scene_id)
    {
        auto scene_link = scene_links.find(scene_id);
        KENGINE_ASSERT_WARN(scene_link != scene_links.end(),
                            "Can't find scene {}",
                            scene_id.get_string());
        if (scene_link != scene_links.end())
        {
            auto temp_sc = scene_manager::load_scene(scene_link->second, this);
            if (temp_sc)
            {
                current_scene    = temp_sc;
                current_scene_id = scene_id;
                current_scene->on_start();
            }
        }
    }

    void game::set_current_scene(std::shared_ptr<scene> sc)
    {
        current_scene = sc;
        current_scene->on_start();
        sc->set_game(this);
    }

    void game::set_current_scene(std::filesystem::path sc_path)
    {
        auto sc = scene_manager::load_scene(sc_path, this);
        if (sc)
        {
            auto name            = sc_path.stem();
            auto name_id         = hash_string(name.string().c_str());
            scene_links[name_id] = std::filesystem::proximate(
                sc_path, scene_manager::assets_base_folder);
            current_scene    = sc;
            current_scene_id = name_id;
            current_scene->on_start();
        }
    }

    std::shared_ptr<scene> game::load_scene(string_id scene_id)
    {
        std::shared_ptr<scene> ret_scene  = nullptr;
        auto                   scene_link = scene_links.find(scene_id);
        if (scene_link != scene_links.end())
        {
            ret_scene = scene_manager::load_scene(scene_link->second, this);
            if (ret_scene)
            {
                ret_scene->on_start();
            }
        }
        return ret_scene;
    }

    std::shared_ptr<scene> game::load_scene(std::filesystem::path sc_path)
    {
        std::shared_ptr<scene> ret_scene = nullptr;
        auto                   sc = scene_manager::load_scene(sc_path, this);
        if (sc)
        {
            auto name            = sc_path.stem();
            auto name_id         = hash_string(name.string().c_str());
            scene_links[name_id] = std::filesystem::proximate(
                sc_path, scene_manager::assets_base_folder);
            ret_scene = sc;
            ret_scene->on_start();
        }
        return ret_scene;
    }

    static std::filesystem::path scene_links_path = "scene-data.kpkg";

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
            std::filesystem::proximate(path, scene_manager::assets_base_folder);

        auto name_id = hash_string(name.data());

        scene_links[name_id] = final_path;
    }

    void game::remove_scene_link(string_id link_id)
    {
        auto sc_link = scene_links.find(link_id);
        if (sc_link != scene_links.end())
        {
            scene_links.erase(sc_link);
        }
    }

    void game::save_scene(string_id scene_id, const scene& sc)
    {
        auto sc_link = scene_links.find(scene_id);
        if (sc_link != scene_links.end())
        {
            auto path    = sc_link->second;
            auto sc_file = file_manager::open_file(
                scene_manager::assets_base_folder / path, std::ios_base::out);
            if (sc_file)
            {
                std::ostream sc_stream(sc_file.get());
                serialization::write(sc_stream, sc);
            }
        }
    }

} // namespace Kengine