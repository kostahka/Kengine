#include "Kengine/resources/shader-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/resources/resource-manager.hxx"

#include <sstream>

namespace Kengine
{
    static GLint             info_len;
    static std::vector<char> info_log;
    static GLint             succes;

    // ------------------------
    // Fragment shader resource
    // ------------------------
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
            KENGINE_INFO("Unloaded fragment shader, {}", fragment_id);
        }
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
            KENGINE_ERROR("Failed to compile fragment shader.");
            return;
        }

        KENGINE_INFO("Loaded and compiled fragment shader: {}",
                     get_string(r_id));
    }

    void fragment_shader_res::unload_data()
    {
        if (fragment_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            KENGINE_INFO("Unloaded fragment shader, {}", fragment_id);
            fragment_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Vertex shader resource
    // ------------------------
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
            KENGINE_INFO("Unloaded vertex shader, {}", vertex_id);
        }
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
            KENGINE_ERROR("Failed to compile vertex shader.");
        }

        KENGINE_INFO("Loaded and compiled vertex shader: {}", vertex_id);
    }

    void vertex_shader_res::unload_data()
    {
        if (vertex_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            KENGINE_INFO("Unloaded vertex shader, {}", vertex_id);
            vertex_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Geometry shader resource
    // ------------------------
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
            KENGINE_INFO("Unloaded geometry shader, {}", geometry_id);
        }
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
            KENGINE_ERROR("Failed to compile geometry shader.");
        }

        KENGINE_INFO("Loaded and compiled geometry shader: {}", geometry_id);
    }

    void geometry_shader_res::unload_data()
    {
        if (geometry_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            KENGINE_INFO("Unloaded geometry shader, {}", geometry_id);
            geometry_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Shader program resource
    // ------------------------
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
            KENGINE_INFO("Unloaded shader program, {}", id);
        }
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
            KENGINE_ERROR("Failed to link shader program. Log: {}",
                          info_log.data());
        }

        KENGINE_INFO("Loaded and compiled shader program: {}", id);

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
            KENGINE_INFO("Unloaded shader program, {}", id);
            id = 0;
        }
    }
} // namespace Kengine