#include "Kengine/graphics/shader-utils/shader-type.hxx"

#include "../../opengl/opengl.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine::graphics
{
    void set_uniform1i(uint32_t location, int value)
    {
        KENGINE_GL_CHECK(glUniform1i(location, value));
    }

    void set_uniform2i(uint32_t location, ivec2 value)
    {
        KENGINE_GL_CHECK(glUniform2i(location, value.x, value.y));
    }

    void set_uniform3i(uint32_t location, ivec3 value)
    {
        KENGINE_GL_CHECK(glUniform3i(location, value.x, value.y, value.z));
    }

    void set_uniform4i(uint32_t location, ivec4 value)
    {
        KENGINE_GL_CHECK(
            glUniform4i(location, value.x, value.y, value.z, value.w));
    }

    void set_uniform1ui(uint32_t location, unsigned int value)
    {
        KENGINE_GL_CHECK(glUniform1ui(location, value));
    }

    void set_uniform2ui(uint32_t location, uvec2 value)
    {
        KENGINE_GL_CHECK(glUniform2ui(location, value.x, value.y));
    }

    void set_uniform3ui(uint32_t location, uvec3 value)
    {
        KENGINE_GL_CHECK(glUniform3ui(location, value.x, value.y, value.z));
    }

    void set_uniform4ui(uint32_t location, uvec4 value)
    {
        KENGINE_GL_CHECK(
            glUniform4ui(location, value.x, value.y, value.z, value.w));
    }

    void set_uniform1f(uint32_t location, float value)
    {
        KENGINE_GL_CHECK(glUniform1f(location, value));
    }

    void set_uniform2f(uint32_t location, vec2 value)
    {
        KENGINE_GL_CHECK(glUniform2f(location, value.x, value.y));
    }

    void set_uniform3f(uint32_t location, vec3 value)
    {
        KENGINE_GL_CHECK(glUniform3f(location, value.x, value.y, value.z));
    }

    void set_uniform4f(uint32_t location, vec4 value)
    {
        KENGINE_GL_CHECK(
            glUniform4f(location, value.x, value.y, value.z, value.w));
    }

    void set_uniform_matrix2x2(uint32_t location, mat2x2 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix2x3(uint32_t location, mat2x3 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix2x4(uint32_t location, mat2x4 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix3x3(uint32_t location, mat3x3 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix3x2(uint32_t location, mat3x2 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix3x4(uint32_t location, mat3x4 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix4x4(uint32_t location, mat4x4 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix4x2(uint32_t location, mat4x2 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }

    void set_uniform_matrix4x3(uint32_t location, mat4x3 value)
    {
        KENGINE_GL_CHECK(
            glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
    }
} // namespace Kengine::graphics