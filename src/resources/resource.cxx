#include "Kengine/resources/resource.hxx"

#include "Kengine/file-last-modify-listener.hxx"
#include "Kengine/resources/resource-manager.hxx"

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

    void resource::reload_data()
    {
        if (p_count)
        {
            this->unload_data();
            this->load_data();
        }
    }

    const char* get_resource_type_str(resource_type type)
    {
        switch (type)
        {
            case resource_type::fragment_shader:
                return "fragment_shader";
            case resource_type::geometry_shader:
                return "geometry_shader";
            case resource_type::vertex_shader:
                return "vertex_shader";
            case resource_type::shader_program:
                return "shader_program";
            case resource_type::framebuffer:
                return "framebuffer";
            case resource_type::material:
                return "material";
            case resource_type::audio:
                return "audio";
            case resource_type::sprite_material:
                return "sprite_material";
            case resource_type::texture:
                return "texture";
            case resource_type::gui_material:
                return "gui_material";
            case resource_type::renderbuffer:
                return "renderbuffer";
        }

        return "undefined";
    }
} // namespace Kengine