#include "Kengine/resources/shader-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/resources/resource-manager.hxx"

#include "imgui.h"

#include <array>
#include <sstream>

namespace Kengine
{
    static GLint             info_len;
    static std::vector<char> info_log;
    static std::vector<char> uniform_name_buffer;
    static GLint             succes;

    // ------------------------
    // Fragment shader resource
    // ------------------------
    fragment_shader_res::fragment_shader_res(std::string_view name)
        : resource(resource_type::fragment_shader, name)
        , code("")
        , fragment_id(0)
    {
    }

    fragment_shader_res::fragment_shader_res(const path&      fragment_file,
                                             std::string_view name)
        : resource(resource_type::fragment_shader, fragment_file, name)
        , fragment_id(0)
    {
    }

    fragment_shader_res::fragment_shader_res(const string_view& fragment_code,
                                             std::string_view   name)
        : resource(resource_type::fragment_shader, name)
        , code(fragment_code)
        , fragment_id(0)
    {
    }

    fragment_shader_res::~fragment_shader_res()
    {
        if (fragment_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            KENGINE_INFO("Unloaded fragment shader: {}", r_id.get_string());
        }
    }

    std::size_t fragment_shader_res::serialize(std::ostream& os) const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::write(os, from_file);
        if (!from_file)
        {
            size += serialization::write(os, code);
        }

        return size;
    }

    std::size_t fragment_shader_res::deserialize(std::istream& is)
    {
        std::size_t size      = 0;
        bool        from_file = true;
        size += serialization::read(is, from_file);
        if (!from_file)
        {
            size += serialization::read(is, code);
            f_path.clear();
        }

        return size;
    }

    std::size_t fragment_shader_res::serialize_size() const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::size(from_file);
        if (!from_file)
        {
            size += serialization::size(code);
        }

        return size;
    }

    void fragment_shader_res::load_data()
    {
        const GLchar* shader_code;
        if (!f_path.empty())
        {
            std::stringstream s_code;
            auto              file_buf = file_manager::load_file(f_path);
            if (file_buf == nullptr)
                return;

            s_code.write(file_buf->rbegin(), file_buf->size());
            code = s_code.str();

            shader_code = code.c_str();
        }
        else
        {
            shader_code = code.c_str();
        }
        fragment_id = KENGINE_GL_CHECK(glCreateShader(GL_FRAGMENT_SHADER));
        KENGINE_GL_CHECK(glShaderSource(fragment_id, 1, &shader_code, nullptr));
        KENGINE_GL_CHECK(glCompileShader(fragment_id));

        KENGINE_GL_CHECK(
            glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &info_len));
        if (info_len > 1)
        {
            info_log.resize(info_len);
            KENGINE_GL_CHECK(glGetShaderInfoLog(
                fragment_id, info_len, nullptr, info_log.data()));
            KENGINE_DEBUG("Fragment shader log: {}", info_log.data());
        }

        glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            fragment_id = 0;
            KENGINE_ERROR("Failed to compile fragment shader: {}",
                          r_id.get_string());
            return;
        }

        KENGINE_INFO("Loaded and compiled fragment shader: {}",
                     r_id.get_string());
    }

    void fragment_shader_res::unload_data()
    {
        if (fragment_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            KENGINE_INFO("Unloaded fragment shader: {}", r_id.get_string());
            fragment_id = 0;
        }
    }

    bool fragment_shader_res::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        ImGui::PopID();
#endif
        return false;
    }

    // ------------------------
    // Vertex shader resource
    // ------------------------
    vertex_shader_res::vertex_shader_res(std::string_view name)
        : resource(resource_type::vertex_shader, name)
        , code("")
        , vertex_id(0)
    {
    }

    vertex_shader_res::vertex_shader_res(const path&      vertex_file,
                                         std::string_view name)
        : resource(resource_type::vertex_shader, vertex_file, name)
        , vertex_id(0)
    {
    }

    vertex_shader_res::vertex_shader_res(const string_view& vertex_code,
                                         std::string_view   name)
        : resource(resource_type::vertex_shader, name)
        , code(vertex_code)
        , vertex_id(0)
    {
    }

    vertex_shader_res::~vertex_shader_res()
    {
        if (vertex_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            KENGINE_INFO("Unloaded vertex shader: {}", r_id.get_string());
        }
    }

    std::size_t vertex_shader_res::serialize(std::ostream& os) const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::write(os, from_file);
        if (!from_file)
        {
            size += serialization::write(os, code);
        }

        return size;
    }

    std::size_t vertex_shader_res::deserialize(std::istream& is)
    {
        std::size_t size      = 0;
        bool        from_file = true;
        size += serialization::read(is, from_file);
        if (!from_file)
        {
            size += serialization::read(is, code);
            f_path.clear();
        }

        return size;
    }

    std::size_t vertex_shader_res::serialize_size() const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::size(from_file);
        if (!from_file)
        {
            size += serialization::size(code);
        }

        return size;
    }

    void vertex_shader_res::load_data()
    {
        const GLchar* shader_code;
        if (!f_path.empty())
        {
            std::stringstream s_code;
            auto              file_buf = file_manager::load_file(f_path);
            if (file_buf == nullptr)
                return;

            s_code.write(file_buf->rbegin(), file_buf->size());
            code = s_code.str();

            shader_code = code.c_str();
        }
        else
        {
            shader_code = code.c_str();
        }

        vertex_id = KENGINE_GL_CHECK(glCreateShader(GL_VERTEX_SHADER));
        KENGINE_GL_CHECK(glShaderSource(vertex_id, 1, &shader_code, nullptr));
        KENGINE_GL_CHECK(glCompileShader(vertex_id));

        KENGINE_GL_CHECK(
            glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &info_len));
        if (info_len > 1)
        {
            info_log.resize(info_len);
            KENGINE_GL_CHECK(glGetShaderInfoLog(
                vertex_id, info_len, nullptr, info_log.data()));
            KENGINE_DEBUG("Vertex shader log: {}", info_log.data());
        }

        glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            vertex_id = 0;
            KENGINE_ERROR("Failed to compile vertex shader: {}",
                          r_id.get_string());
        }

        KENGINE_INFO("Loaded and compiled vertex shader: {}",
                     r_id.get_string());
    }

    void vertex_shader_res::unload_data()
    {
        if (vertex_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            KENGINE_INFO("Unloaded vertex shader: {}", r_id.get_string());
            vertex_id = 0;
        }
    }

    bool vertex_shader_res::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        ImGui::PopID();
#endif
        return false;
    }

    // ------------------------
    // Geometry shader resource
    // ------------------------
    geometry_shader_res::geometry_shader_res(std::string_view name)
        : resource(resource_type::geometry_shader, name)
        , code("")
        , geometry_id(0)
    {
    }

    geometry_shader_res::geometry_shader_res(const path&      geometry_file,
                                             std::string_view name)
        : resource(resource_type::geometry_shader, geometry_file, name)
        , geometry_id(0)
    {
    }

    geometry_shader_res::geometry_shader_res(const string_view& geometry_code,
                                             std::string_view   name)
        : resource(resource_type::geometry_shader, name)
        , code(geometry_code)
        , geometry_id(0)
    {
    }

    geometry_shader_res::~geometry_shader_res()
    {
        if (geometry_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            KENGINE_INFO("Unloaded geometry shader: {}", r_id.get_string());
        }
    }

    std::size_t geometry_shader_res::serialize(std::ostream& os) const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::write(os, from_file);
        if (!from_file)
        {
            size += serialization::write(os, code);
        }

        return size;
    }

    std::size_t geometry_shader_res::deserialize(std::istream& is)
    {
        std::size_t size      = 0;
        bool        from_file = true;
        size += serialization::read(is, from_file);
        if (!from_file)
        {
            size += serialization::read(is, code);
            f_path.clear();
        }

        return size;
    }

    std::size_t geometry_shader_res::serialize_size() const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();
        size += serialization::size(from_file);
        if (!from_file)
        {
            size += serialization::size(code);
        }

        return size;
    }

    void geometry_shader_res::load_data()
    {
        const GLchar* shader_code;
        if (!f_path.empty())
        {
            std::stringstream s_code;
            auto              file_buf = file_manager::load_file(f_path);
            if (file_buf == nullptr)
                return;

            s_code.write(file_buf->rbegin(), file_buf->size());
            code = s_code.str();

            shader_code = code.c_str();
        }
        else
        {
            shader_code = code.c_str();
        }

        geometry_id = KENGINE_GL_CHECK(glCreateShader(GL_GEOMETRY_SHADER));
        KENGINE_GL_CHECK(glShaderSource(geometry_id, 1, &shader_code, nullptr));
        KENGINE_GL_CHECK(glCompileShader(geometry_id));

        KENGINE_GL_CHECK(
            glGetShaderiv(geometry_id, GL_INFO_LOG_LENGTH, &info_len));
        if (info_len > 1)
        {
            info_log.resize(info_len);
            KENGINE_GL_CHECK(glGetShaderInfoLog(
                geometry_id, info_len, nullptr, info_log.data()));
            KENGINE_DEBUG("Geometry shader log: {}", info_log.data());
        }

        glGetShaderiv(geometry_id, GL_COMPILE_STATUS, &succes);
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            geometry_id = 0;
            KENGINE_ERROR("Failed to compile geometry shader: {}",
                          r_id.get_string());
        }

        KENGINE_INFO("Loaded and compiled geometry shader: {}",
                     r_id.get_string());
    }

    void geometry_shader_res::unload_data()
    {
        if (geometry_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            KENGINE_INFO("Unloaded geometry shader: {}", r_id.get_string());
            geometry_id = 0;
        }
    }

    bool geometry_shader_res::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        ImGui::PopID();
#endif
        return edited;
    }

    // ------------------------
    // Shader program resource
    // ------------------------
    shader_res::shader_res(std::string_view name)
        : resource(resource_type::shader_program, name)
        , vertex_res(nullptr)
        , geometry_res(nullptr)
        , fragment_res(nullptr)
        , id(0)
    {
    }

    shader_res::shader_res(const res_ptr<vertex_shader_res>&   vert,
                           const res_ptr<geometry_shader_res>& geom,
                           const res_ptr<fragment_shader_res>& frag,
                           std::string_view                    name)
        : resource(resource_type::shader_program, name)
        , vertex_res(vert)
        , geometry_res(geom)
        , fragment_res(frag)
        , id(0)
    {
    }

    shader_res::shader_res(const res_ptr<vertex_shader_res>&   vert,
                           const res_ptr<fragment_shader_res>& frag,
                           std::string_view                    name)
        : resource(resource_type::shader_program, name)
        , vertex_res(vert)
        , fragment_res(frag)
        , geometry_res(nullptr)
        , id(0)
    {
    }

    shader_res::shader_res(const string_view& vertex_code,
                           const string_view& fragment_code,
                           std::string_view   name)
        : resource(resource_type::shader_program, name)
        , geometry_res(nullptr)
        , id(0)
    {
        std::string str_name(name);
        std::string v_name = str_name + "_vertex";
        std::string f_name = str_name + "_fragment";

        vertex_res   = make_resource<vertex_shader_res>(vertex_code,
                                                      std::string_view(v_name));
        fragment_res = make_resource<fragment_shader_res>(
            fragment_code, std::string_view(f_name));
    }

    shader_res::shader_res(const string_view& vertex_code,
                           const string_view& geometry_code,
                           const string_view& fragment_code,
                           std::string_view   name)
        : resource(resource_type::shader_program, name)
        , id(0)
    {
        std::string str_name(name);
        std::string v_name = str_name + "_vertex";
        std::string g_name = str_name + "_geometry";
        std::string f_name = str_name + "_fragment";

        vertex_res   = make_resource<vertex_shader_res>(vertex_code,
                                                      std::string_view(v_name));
        geometry_res = make_resource<geometry_shader_res>(
            geometry_code, std::string_view(g_name));
        fragment_res = make_resource<fragment_shader_res>(
            fragment_code, std::string_view(f_name));
    }

    shader_res::shader_res(const path&      vertex_path,
                           const path&      fragment_path,
                           std::string_view name)
        : resource(resource_type::shader_program, name)
        , geometry_res(nullptr)
        , id(0)
    {
        std::string str_name(name);
        std::string v_name = str_name + "_vertex";
        std::string f_name = str_name + "_fragment";

        vertex_res   = make_resource<vertex_shader_res>(vertex_path,
                                                      std::string_view(v_name));
        fragment_res = make_resource<fragment_shader_res>(
            fragment_path, std::string_view(f_name));
    }

    shader_res::shader_res(const path&      vertex_path,
                           const path&      geometry_path,
                           const path&      fragment_path,
                           std::string_view name)
        : resource(resource_type::shader_program, name)
        , id(0)
    {
        std::string str_name(name);
        std::string v_name = str_name + "_vertex";
        std::string g_name = str_name + "_geometry";
        std::string f_name = str_name + "_fragment";

        vertex_res   = make_resource<vertex_shader_res>(vertex_path,
                                                      std::string_view(v_name));
        geometry_res = make_resource<geometry_shader_res>(
            geometry_path, std::string_view(g_name));
        fragment_res = make_resource<fragment_shader_res>(
            fragment_path, std::string_view(f_name));
    }

    shader_res::~shader_res()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteProgram(id));
            KENGINE_INFO("Unloaded shader program: {}", r_id.get_string());
        }
    }

    std::size_t shader_res::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, vertex_res);
        size += serialization::write(os, geometry_res);
        size += serialization::write(os, fragment_res);
        size += serialization::write(os, uniform_block_bindings);

        return size;
    }

    std::size_t shader_res::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, vertex_res);
        size += serialization::read(is, geometry_res);
        size += serialization::read(is, fragment_res);
        size += serialization::read(is, uniform_block_bindings);

        return size;
    }

    std::size_t shader_res::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(vertex_res);
        size += serialization::size(geometry_res);
        size += serialization::size(fragment_res);
        size += serialization::size(uniform_block_bindings);

        return size;
    }

    void shader_res::load_data()
    {

        id = KENGINE_GL_CHECK(glCreateProgram());

        if (vertex_res)
        {
            vertex_res->take_data();
            KENGINE_GL_CHECK(glAttachShader(id, vertex_res->get_id()));
        }
        if (geometry_res)
        {
            geometry_res->take_data();
            KENGINE_GL_CHECK(glAttachShader(id, geometry_res->get_id()));
        }
        if (fragment_res)
        {
            fragment_res->take_data();
            KENGINE_GL_CHECK(glAttachShader(id, fragment_res->get_id()));
        }

        KENGINE_GL_CHECK(glLinkProgram(id));
        KENGINE_GL_CHECK(glGetProgramiv(id, GL_LINK_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &info_len));
            info_log.resize(info_len);
            KENGINE_GL_CHECK(
                glGetProgramInfoLog(id, info_len, nullptr, info_log.data()));
            KENGINE_GL_CHECK(glDeleteProgram(id));
            id = 0;
            KENGINE_ERROR("Failed to link shader program: {}. Log: {}",
                          r_id.get_string(),
                          info_log.data());
        }
        else
        {
            int32_t uniforms_count     = 0;
            int32_t uniform_max_length = 0;
            KENGINE_GL_CHECK(
                glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniforms_count));
            KENGINE_GL_CHECK(glGetProgramiv(
                id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_length));
            uniform_name_buffer.resize(uniform_max_length);
            for (int32_t i = 0; i < uniforms_count; i++)
            {
                int32_t uniform_name_length = 0;
                int32_t uniform_size        = 0;
                GLenum  uniform_type        = 0;
                KENGINE_GL_CHECK(
                    glGetActiveUniform(id,
                                       i,
                                       100,
                                       &uniform_name_length,
                                       &uniform_size,
                                       &uniform_type,
                                       uniform_name_buffer.data()));

                string_id uniform_name_id =
                    hash_string(uniform_name_buffer.data());

                uint32_t uniform_location = KENGINE_GL_CHECK(
                    glGetUniformLocation(id, uniform_name_buffer.data()));

                uniform_locations[uniform_name_id] = uniform_location;
            }

            for (auto& [uniform_block_name, uniform_block_binding] :
                 uniform_block_bindings)
            {
                auto block_index = KENGINE_GL_CHECK(
                    glGetUniformBlockIndex(id, uniform_block_name.data()));
                KENGINE_GL_CHECK(glUniformBlockBinding(
                    id, block_index, uniform_block_binding));
            }
        }

        KENGINE_INFO("Loaded and compiled shader program: {}",
                     r_id.get_string());

        if (vertex_res)
            vertex_res->free_data();
        if (geometry_res)
            geometry_res->free_data();
        if (fragment_res)
            fragment_res->free_data();
    }

    void shader_res::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteProgram(id));
            KENGINE_INFO("Unloaded shader program: {}", r_id.get_string());
            id = 0;
            uniform_locations.clear();
        }
    }

    void shader_res::set_uniform_block_binding(std::string_view name,
                                               uint32_t         binding)
    {
        uniform_block_bindings[std::string(name)] = binding;
        if (p_count > 0)
        {
            auto block_index =
                KENGINE_GL_CHECK(glGetUniformBlockIndex(id, name.data()));
            KENGINE_GL_CHECK(glUniformBlockBinding(id, block_index, binding));
        }
    }

    bool shader_res::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        edited = edited || imgui::edit_resource("Vertex resource", &vertex_res);
        edited =
            edited || imgui::edit_resource("Geometry resource", &geometry_res);
        edited =
            edited || imgui::edit_resource("Fragment resource", &fragment_res);

        ImGui::PopID();
#endif
        return edited;
    }

} // namespace Kengine