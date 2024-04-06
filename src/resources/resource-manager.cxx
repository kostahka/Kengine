#include "Kengine/resources/resource-manager.hxx"

#include "../engine.hxx"
#include "../scene/scene-manager.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/resources/audio-resource.hxx"
#include "Kengine/resources/framebuffer-resource.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"

#include <unordered_map>

namespace Kengine::resource_manager
{
    static std::unordered_map<string_id, res_weak_ptr<resource>>
        registered_resources;

    void initialize() {}

    void shutdown()
    {
        while (registered_resources.size() > 0)
        {
            registered_resources.begin()->second.counter->free();
        }
    }

    res_ptr<resource> load_resource(path res_path)
    {
        auto res_file = file_manager::load_file(
            scene_manager::assets_base_folder / res_path);
        if (res_file && res_file->size() > 0)
        {
            std::istream res_is(res_file.get());

            res_ptr<resource> loaded_res = nullptr;
            std::size_t       size       = 0;

            std::string   res_name;
            resource_type r_type;
            std::string   res_file_path;

            size += serialization::read(res_is, res_name);
            size += serialization::read(res_is, r_type);
            size += serialization::read(res_is, res_file_path);

            loaded_res = get_resource(string_id(res_name.c_str()));

            if (loaded_res)
                return loaded_res;

            path f_path(res_file_path);
            switch (r_type)
            {
                case resource_type::fragment_shader:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<fragment_shader_res>(
                            res_path, f_path, res_name));
                    break;
                case resource_type::vertex_shader:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<vertex_shader_res>(
                            res_path, f_path, res_name));
                    break;
                case resource_type::geometry_shader:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<geometry_shader_res>(
                            res_path, f_path, res_name));
                    break;
                case resource_type::shader_program:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<shader_res>(res_path,
                                                            res_name));
                    break;
                case resource_type::material:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<material_resource>(res_path,
                                                                   res_name));
                    break;
                case resource_type::audio:
                    loaded_res = loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<audio_resource>(res_path,
                                                                res_name));
                    break;
                case resource_type::sprite_material:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<sprite_material_resource>(
                            res_path, res_name));
                    break;
                case resource_type::gui_material:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<gui_material_resource>(
                            res_path, res_name));
                    break;
                case resource_type::texture:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<texture_resource>(
                            res_path, f_path, res_name));
                    break;
                case resource_type::renderbuffer:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<renderbuffer_resource>(
                            res_path, res_name));
                    break;
                case resource_type::framebuffer:
                    loaded_res = static_resource_cast<resource>(
                        make_resource_from_file<framebuffer_resource>(
                            res_path, res_name));
                    break;
            }
            size += loaded_res->deserialize(res_is);

            if (size > 0)
                return loaded_res;
        }
        return nullptr;
    }

    void save_resource(res_ptr<resource> res)
    {
        if (res)
        {
            path res_path = res.get_res_path();
            KENGINE_ASSERT(!res_path.empty(),
                           "Saving resource provided for resources created "
                           "with 'make_resource_from_file'");
            if (!res_path.empty())
            {
                auto res_file = file_manager::open_file(
                    scene_manager::assets_base_folder / res_path,
                    std::ios_base::out);
                std::ostream res_os(res_file.get());

                std::size_t size = 0;
                size += serialization::write(
                    res_os, std::string(get_string(res->get_resource_id())));
                size += serialization::write(res_os, res->get_resource_type());
                size += serialization::write(res_os, res->get_file_path());
                size += serialization::write(res_os, *res);
            }
        }
    }

    res_ptr<resource> get_resource(string_id r_id)
    {
        auto resource = registered_resources.find(r_id);
        if (resource != registered_resources.end())
            return resource->second.lock();
        else
            return nullptr;
    }

    void registrate_resource(const res_ptr<resource>& res)
    {
        if (res)
        {
            string_id r_id = res->get_resource_id();

            KENGINE_ASSERT(!registered_resources.contains(r_id),
                           "There's another resource with such id to add");

            KENGINE_ASSERT(r_id, "Invalid resource.");

            if (r_id)
                registered_resources[r_id] = res_weak_ptr<resource>(res);

            if (e_game)
                e_game->get_current_scene().add_resource(res);
        }
    }

    void remove_resource(string_id r_id)
    {
        KENGINE_ASSERT(registered_resources.contains(r_id),
                       "There's no resource with such id to remove");
        if (registered_resources.contains(r_id))
            registered_resources.erase(r_id);
    }

    path relative_assets_path(path full_path)
    {
        return std::filesystem::proximate(full_path,
                                          scene_manager::assets_base_folder);
    }

} // namespace Kengine::resource_manager