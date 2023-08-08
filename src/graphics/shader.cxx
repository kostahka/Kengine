#include "Kengine/graphics/shader.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"

#include "glm/gtc/type_ptr.hpp"

namespace Kengine::graphics
{
    static GLint             info_len;
    static std::vector<char> info_log;
    static GLint             succes;

    std::shared_ptr<shader> shader::load_from_files(
        std::string_view vertex_path, std::string_view fragment_path)
    {
        std::stringstream vertex_code;
        std::stringstream fragment_code;

        file_manager::membuf vertex_file = file_manager::load_file(vertex_path);
        file_manager::membuf fragment_file =
            file_manager::load_file(fragment_path);

        if (!vertex_file.size() || !fragment_file.size())
        {
            KENGINE_ERROR("No data from files for shader. Vertex size: {}. "
                          "Fragment size: {}",
                          vertex_file.size(),
                          fragment_file.size());
            return nullptr;
        }

        vertex_code.write(vertex_file.begin(), vertex_file.size());
        fragment_code.write(fragment_file.begin(), fragment_file.size());

        return std::shared_ptr<shader>(
            new shader(vertex_code.str(), fragment_code.str()));
    }

    std::shared_ptr<shader> shader::load_from_files(
        std::string_view vertex_path,
        std::string_view geometry_path,
        std::string_view fragment_path)
    {
        std::stringstream vertex_code;
        std::stringstream geometry_code;
        std::stringstream fragment_code;

        file_manager::membuf vertex_file = file_manager::load_file(vertex_path);
        file_manager::membuf geometry_file =
            file_manager::load_file(geometry_path);
        file_manager::membuf fragment_file =
            file_manager::load_file(fragment_path);

        if (!vertex_file.size() || !fragment_file.size() ||
            !geometry_file.size())
        {
            KENGINE_ERROR("No data from files for shader. Vertex size: {}. "
                          "Geometry size: {}. "
                          "Fragment size: {}",
                          vertex_file.size(),
                          geometry_file.size(),
                          fragment_file.size());
            return nullptr;
        }

        vertex_code.write(vertex_file.begin(), vertex_file.size());
        geometry_code.write(geometry_file.begin(), geometry_file.size());
        fragment_code.write(fragment_file.begin(), fragment_file.size());

        return std::shared_ptr<shader>(new shader(
            vertex_code.str(), geometry_code.str(), fragment_code.str()));
    }

    shader::shader(std::string_view vertex,
                   std::string_view geometry,
                   std::string_view fragment)
        : program(0)
    {
        uint32_t vertex_id, fragment_id, geometry_id;

        vertex_id   = load_vertex(vertex);
        fragment_id = load_fragment(fragment);
        geometry_id = load_geometry(geometry);

        program = KENGINE_GL_CHECK(glCreateProgram());
        KENGINE_GL_CHECK(glAttachShader(program, vertex_id));
        KENGINE_GL_CHECK(glAttachShader(program, fragment_id));
        KENGINE_GL_CHECK(glAttachShader(program, geometry_id));

        KENGINE_GL_CHECK(glLinkProgram(program));

        KENGINE_GL_CHECK(
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len));
        if (info_len > 1)
        {
            info_log.resize(info_len);
            KENGINE_GL_CHECK(glGetProgramInfoLog(
                program, info_len, nullptr, info_log.data()));
            KENGINE_DEBUG("Shader program log: {}", info_log.data());
        }

        KENGINE_GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteProgram(program));
            KENGINE_ERROR("Failed to link shader program.");
        }

        KENGINE_GL_CHECK(glDeleteShader(vertex_id));
        KENGINE_GL_CHECK(glDeleteShader(fragment_id));
    }

    shader::shader(std::string_view vertex, std::string_view fragment)
        : program(0)
    {
        uint32_t vertex_id, fragment_id;

        vertex_id   = load_vertex(vertex);
        fragment_id = load_fragment(fragment);

        program = KENGINE_GL_CHECK(glCreateProgram());
        KENGINE_GL_CHECK(glAttachShader(program, vertex_id));
        KENGINE_GL_CHECK(glAttachShader(program, fragment_id));

        KENGINE_GL_CHECK(glLinkProgram(program));

        KENGINE_GL_CHECK(
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len));
        if (info_len > 1)
        {
            info_log.resize(info_len);
            KENGINE_GL_CHECK(glGetProgramInfoLog(
                program, info_len, nullptr, info_log.data()));
            KENGINE_DEBUG("Shader program log: {}", info_log.data());
        }

        KENGINE_GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteProgram(program));
            KENGINE_ERROR("Failed to link shader program.");
        }

        KENGINE_GL_CHECK(glDeleteShader(vertex_id));
        KENGINE_GL_CHECK(glDeleteShader(fragment_id));
    }

    shader::~shader()
    {
        if (program)
            KENGINE_GL_CHECK(glDeleteProgram(program));
    }

    void shader::use()
    {
        KENGINE_GL_CHECK(glUseProgram(program));
    }

    void shader::uniform1f(std::string_view name, float v0)
    {
        KENGINE_GL_CHECK(glUniform1f(get_uniform_location(name), v0));
    }

    void shader::uniform2f(std::string_view name, float v0, float v1)
    {
        KENGINE_GL_CHECK(glUniform2f(get_uniform_location(name), v0, v1));
    }

    void shader::uniform3f(std::string_view name, float v0, float v1, float v2)
    {
        KENGINE_GL_CHECK(glUniform3f(get_uniform_location(name), v0, v1, v2));
    }

    void shader::uniform4f(
        std::string_view name, float v0, float v1, float v2, float v3)
    {
        KENGINE_GL_CHECK(
            glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
    }

    void shader::uniform1i(std::string_view name, int v0)
    {
        KENGINE_GL_CHECK(glUniform1i(get_uniform_location(name), v0));
    }

    void shader::uniform2i(std::string_view name, int v0, int v1)
    {
        KENGINE_GL_CHECK(glUniform2i(get_uniform_location(name), v0, v1));
    }

    void shader::uniform3i(std::string_view name, int v0, int v1, int v2)
    {
        KENGINE_GL_CHECK(glUniform3i(get_uniform_location(name), v0, v1, v2));
    }

    void shader::uniform4i(
        std::string_view name, int v0, int v1, int v2, int v3)
    {
        KENGINE_GL_CHECK(
            glUniform4i(get_uniform_location(name), v0, v1, v2, v3));
    }

    void shader::uniform1ui(std::string_view name, unsigned int v0)
    {
        KENGINE_GL_CHECK(glUniform1ui(get_uniform_location(name), v0));
    }

    void shader::uniform2ui(std::string_view name,
                            unsigned int     v0,
                            unsigned int     v1)
    {
        KENGINE_GL_CHECK(glUniform2ui(get_uniform_location(name), v0, v1));
    }

    void shader::uniform3ui(std::string_view name,
                            unsigned int     v0,
                            unsigned int     v1,
                            unsigned int     v2)
    {
        KENGINE_GL_CHECK(glUniform3ui(get_uniform_location(name), v0, v1, v2));
    }

    void shader::uniform4ui(std::string_view name,
                            unsigned int     v0,
                            unsigned int     v1,
                            unsigned int     v2,
                            unsigned int     v3)
    {
        KENGINE_GL_CHECK(
            glUniform4ui(get_uniform_location(name), v0, v1, v2, v3));
    }

    void shader::uniform_matrix2fv(std::string_view name, const glm::mat2& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix2fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix3fv(std::string_view name, const glm::mat3& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix3fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix4fv(std::string_view name, const glm::mat4& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix4fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix2x3fv(std::string_view   name,
                                     const glm::mat2x3& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix2x3fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix3x2fv(std::string_view   name,
                                     const glm::mat3x2& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix3x2fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix2x4fv(std::string_view   name,
                                     const glm::mat2x4& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix2x4fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix4x2fv(std::string_view   name,
                                     const glm::mat4x2& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix4x2fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix3x4fv(std::string_view   name,
                                     const glm::mat3x4& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix3x4fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::uniform_matrix4x3fv(std::string_view   name,
                                     const glm::mat4x3& mat)
    {
        KENGINE_GL_CHECK(glUniformMatrix4x3fv(
            get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(mat)));
    }

    void shader::save_uniform_location(std::string_view name)
    {
        uniform_locations[name] =
            KENGINE_GL_CHECK(glGetUniformLocation(program, name.data()));
    }

    uint32_t shader::load_vertex(std::string_view vertex)
    {
        uint32_t vertex_id;

        const GLchar* vertex_code = vertex.data();

        vertex_id = KENGINE_GL_CHECK(glCreateShader(GL_VERTEX_SHADER));
        KENGINE_GL_CHECK(glShaderSource(vertex_id, 1, &vertex_code, nullptr));
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

        KENGINE_GL_CHECK(glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(vertex_id));
            KENGINE_ERROR("Failed to compile vertex shader.");

            return 0;
        }

        return vertex_id;
    }

    uint32_t shader::load_fragment(std::string_view fragment)
    {
        uint32_t fragment_id;

        const GLchar* fragment_code = fragment.data();

        fragment_id = KENGINE_GL_CHECK(glCreateShader(GL_FRAGMENT_SHADER));
        KENGINE_GL_CHECK(
            glShaderSource(fragment_id, 1, &fragment_code, nullptr));
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

        KENGINE_GL_CHECK(
            glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(fragment_id));
            KENGINE_ERROR("Failed to compile fragment shader.");

            return 0;
        }

        return fragment_id;
    }

    uint32_t shader::load_geometry(std::string_view geometry)
    {
        uint32_t geometry_id;

        const GLchar* geometry_code = geometry.data();

        geometry_id = KENGINE_GL_CHECK(glCreateShader(GL_GEOMETRY_SHADER));
        KENGINE_GL_CHECK(
            glShaderSource(geometry_id, 1, &geometry_code, nullptr));
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

        KENGINE_GL_CHECK(
            glGetShaderiv(geometry_id, GL_COMPILE_STATUS, &succes));
        if (!succes)
        {
            KENGINE_GL_CHECK(glDeleteShader(geometry_id));
            KENGINE_ERROR("Failed to compile geometry shader.");

            return 0;
        }

        return geometry_id;
    }

    uint32_t shader::get_uniform_location(std::string_view name)
    {
        KENGINE_ASSERT(uniform_locations.contains(name), "No such uniform");
        return uniform_locations[name];
    }
} // namespace Kengine::graphics