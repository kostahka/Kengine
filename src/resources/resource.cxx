#include "Kengine/resources/resource.hxx"

#include "Kengine/file-last-modify-listener.hxx"
#include "Kengine/resources/framebuffer-resource.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"

namespace Kengine
{
    void resource::take_data()
    {
        if (!p_count)
        {
            this->load_data();
        }
        p_count++;
    }

    void resource::free_data()
    {
        if (p_count == 1)
        {
            p_count--;
            this->unload_data();
        }
    }

    std::size_t resource::serialize(std::ostream&            os,
                                    const res_ptr<resource>& resource)
    {
        std::size_t size = 0;
        if (resource)
        {
            size += serialization::write(
                os, std::string(get_string(resource->r_id)));
            size += serialization::write(os, resource->r_type);
            size += serialization::write(os, resource->f_path.string());

            size += resource->serialize(os);
        }

        return size;
    }

    std::size_t resource::deserialize(std::istream& is, res_ptr<resource>& res)
    {
        std::size_t size = 0;

        std::string   res_name;
        resource_type r_type;
        std::string   res_path;

        size += serialization::read(is, res_name);
        size += serialization::read(is, r_type);
        size += serialization::read(is, res_path);

        path res_file(res_path);
        switch (r_type)
        {
            case resource_type::fragment_shader:
                res = static_resource_cast<resource>(
                    make_resource<fragment_shader_res>(res_file, res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::vertex_shader:
                res = static_resource_cast<resource>(
                    make_resource<vertex_shader_res>(res_file, res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::geometry_shader:
                res = static_resource_cast<resource>(
                    make_resource<geometry_shader_res>(res_file, res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::shader_program:
                res = static_resource_cast<resource>(
                    make_resource<shader_res>(res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::material:
                res = static_resource_cast<resource>(
                    make_resource<material_resource>(res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::sound:
                return -1;
            case resource_type::sprite_material:
                res = static_resource_cast<resource>(
                    make_resource<sprite_material_resource>(res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::texture:
                res = static_resource_cast<resource>(
                    make_resource<texture_resource>(res_file, res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::renderbuffer:
                res = static_resource_cast<resource>(
                    make_resource<renderbuffer_resource>(res_name));
                size += res->deserialize(is);
                return size;
            case resource_type::framebuffer:
                res = static_resource_cast<resource>(
                    make_resource<framebuffer_resource>(res_name));
                size += res->deserialize(is);
                return size;
        }

        return size;
    }

    resource::resource(resource_type r_type, path f_path, std::string_view name)
        : r_type(r_type)
        , f_path(f_path)
        , p_count(0)
    {
        r_id = hash_string(name.data());
    }

    resource::resource(resource_type r_type, std::string_view name)
        : r_type(r_type)
        , f_path(path())
        , p_count(0)
    {
        r_id = hash_string(name.data());
    }

    resource::~resource()
    {
        resource_manager::remove_resource(r_id);
    }
} // namespace Kengine