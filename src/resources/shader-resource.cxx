#include "Kengine/resources/shader-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"

#include <sstream>

namespace Kengine
{
    static GLint             info_len;
    static std::vector<char> info_log;
    static GLint             succes;

    // ------------------------
    // Fragment shader resource
    // ------------------------
    fragment_shader_res::fragment_shader_res(const path& fragment_file)
        : resource(resource_type::fragment_shader, fragment_file)
        , fragment_id(0)
    {
    }

    fragment_shader_res::fragment_shader_res(const string_view& fragment_code)
        : resource(resource_type::fragment_shader)
        , code(fragment_code)
        , fragment_id(0)
    {
        const GLchar* shader_code = code.c_str();

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
        }

        p_count++;
    }

    fragment_shader_res::~fragment_shader_res()
    {
        if (fragment_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
        }
    }

    void fragment_shader_res::load_data()
    {
        std::stringstream s_code;
        auto              file_buf = file_manager::load_file(f_path);
        if (file_buf == nullptr)
            return;

        s_code.write(file_buf->rbegin(), file_buf->size());
        code = s_code.str();

        const GLchar* shader_code = code.c_str();

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
        }
    }

    void fragment_shader_res::unload_data()
    {
        if (fragment_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            fragment_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Vertex shader resource
    // ------------------------
    vertex_shader_res::vertex_shader_res(const path& vertex_file)
        : resource(resource_type::vertex_shader, vertex_file)
        , vertex_id(0)
    {
    }

    vertex_shader_res::vertex_shader_res(const string_view& vertex_code)
        : resource(resource_type::vertex_shader)
        , code(vertex_code)
        , vertex_id(0)
    {
        const GLchar* shader_code = code.c_str();

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

        p_count++;
    }

    vertex_shader_res::~vertex_shader_res()
    {
        if (vertex_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
        }
    }

    void vertex_shader_res::load_data()
    {
        std::stringstream s_code;
        auto              file_buf = file_manager::load_file(f_path);
        if (file_buf == nullptr)
            return;

        s_code.write(file_buf->rbegin(), file_buf->size());
        code = s_code.str();

        const GLchar* shader_code = code.c_str();

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
    }

    void vertex_shader_res::unload_data()
    {
        if (vertex_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            vertex_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Geometry shader resource
    // ------------------------
    geometry_shader_res::geometry_shader_res(const path& geometry_file)
        : resource(resource_type::geometry_shader, geometry_file)
        , geometry_id(0)
    {
    }

    geometry_shader_res::geometry_shader_res(const string_view& geometry_code)
        : resource(resource_type::geometry_shader)
        , code(geometry_code)
        , geometry_id(0)
    {
        const GLchar* shader_code = code.c_str();

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

        p_count++;
    }

    geometry_shader_res::~geometry_shader_res()
    {
        if (geometry_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
        }
    }

    void geometry_shader_res::load_data()
    {
        std::stringstream s_code;
        auto              file_buf = file_manager::load_file(f_path);
        if (file_buf == nullptr)
            return;

        s_code.write(file_buf->rbegin(), file_buf->size());
        code = s_code.str();

        const GLchar* shader_code = code.c_str();

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
    }

    void geometry_shader_res::unload_data()
    {
        if (geometry_id)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            geometry_id = 0;
        }
        code.clear();
    }

    // ------------------------
    // Shader program resource
    // ------------------------
    shader_res::shader_res(const shared_ptr<vertex_shader_res>&   vert,
                           const shared_ptr<geometry_shader_res>& geom,
                           const shared_ptr<fragment_shader_res>& frag)
        : resource(resource_type::shader_program)
        , vertex_res(vert)
        , geometry_res(geom)
        , fragment_res(frag)
        , id(0)
    {
    }

    shader_res::shader_res(const shared_ptr<vertex_shader_res>&   vert,
                           const shared_ptr<fragment_shader_res>& frag)
        : resource(resource_type::shader_program)
        , vertex_res(vert)
        , fragment_res(frag)
        , geometry_res(nullptr)
        , id(0)
    {
    }

    shader_res::shader_res(const string_view& vertex_code,
                           const string_view& fragment_code)
        : resource(resource_type::shader_program)
        , geometry_res(nullptr)
        , id(0)
    {
        vertex_res   = std::make_shared<vertex_shader_res>(vertex_code);
        fragment_res = std::make_shared<fragment_shader_res>(fragment_code);
    }

    shader_res::shader_res(const string_view& vertex_code,
                           const string_view& geometry_code,
                           const string_view& fragment_code)
        : resource(resource_type::shader_program)
        , id(0)
    {
        vertex_res   = std::make_shared<vertex_shader_res>(vertex_code);
        geometry_res = std::make_shared<geometry_shader_res>(geometry_code);
        fragment_res = std::make_shared<fragment_shader_res>(fragment_code);
    }

    shader_res::shader_res(const path& vertex_path, const path& fragment_path)
        : resource(resource_type::shader_program)
        , geometry_res(nullptr)
        , id(0)
    {
        vertex_res   = std::make_shared<vertex_shader_res>(vertex_path);
        fragment_res = std::make_shared<fragment_shader_res>(fragment_path);
    }

    shader_res::shader_res(const path& vertex_path,
                           const path& geometry_path,
                           const path& fragment_path)
        : resource(resource_type::shader_program)
        , id(0)
    {
        vertex_res   = std::make_shared<vertex_shader_res>(vertex_path);
        geometry_res = std::make_shared<geometry_shader_res>(geometry_path);
        fragment_res = std::make_shared<fragment_shader_res>(fragment_path);
    }

    shader_res::~shader_res()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteProgram(id));
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
            id = 0;
        }
    }
} // namespace Kengine