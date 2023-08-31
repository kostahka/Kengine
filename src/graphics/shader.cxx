#include "Kengine/graphics/shader.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"

#include "glm/gtc/type_ptr.hpp"

#include <sstream>

namespace Kengine::graphics
{
    shader::shader(const std::shared_ptr<shader_res>& shader_resource)
        : resource(shader_resource)
    {
        resource->take_data();
    }

    shader::~shader()
    {
        resource->free_data();
    }

    void shader::use()
    {
        KENGINE_GL_CHECK(glUseProgram(resource->get_id()));
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
        uniform_locations[name] = KENGINE_GL_CHECK(
            glGetUniformLocation(resource->get_id(), name.data()));
    }

    uint32_t shader::get_uniform_location(std::string_view name)
    {
        KENGINE_ASSERT(uniform_locations.contains(name), "No such uniform");
        return uniform_locations[name];
    }
} // namespace Kengine::graphics