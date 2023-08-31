#pragma once

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "glm/glm.hpp"

#include "Kengine/resources/shader-resource.hxx"

namespace Kengine::graphics
{
    class shader
    {
    public:
        shader(const std::shared_ptr<shader_res>& shader_resource);

        ~shader();

        void use();

        void uniform1f(std::string_view name, float v0);
        void uniform2f(std::string_view name, float v0, float v1);
        void uniform3f(std::string_view name, float v0, float v1, float v2);
        void uniform4f(
            std::string_view name, float v0, float v1, float v2, float v3);

        void uniform1i(std::string_view name, int v0);
        void uniform2i(std::string_view name, int v0, int v1);
        void uniform3i(std::string_view name, int v0, int v1, int v2);
        void uniform4i(std::string_view name, int v0, int v1, int v2, int v3);

        void uniform1ui(std::string_view name, unsigned int v0);
        void uniform2ui(std::string_view name,
                        unsigned int     v0,
                        unsigned int     v1);
        void uniform3ui(std::string_view name,
                        unsigned int     v0,
                        unsigned int     v1,
                        unsigned int     v2);
        void uniform4ui(std::string_view name,
                        unsigned int     v0,
                        unsigned int     v1,
                        unsigned int     v2,
                        unsigned int     v3);

        void uniform_matrix2fv(std::string_view name, const glm::mat2& mat);
        void uniform_matrix3fv(std::string_view name, const glm::mat3& mat);
        void uniform_matrix4fv(std::string_view name, const glm::mat4& mat);

        void uniform_matrix2x3fv(std::string_view name, const glm::mat2x3& mat);
        void uniform_matrix3x2fv(std::string_view name, const glm::mat3x2& mat);
        void uniform_matrix2x4fv(std::string_view name, const glm::mat2x4& mat);
        void uniform_matrix4x2fv(std::string_view name, const glm::mat4x2& mat);
        void uniform_matrix3x4fv(std::string_view name, const glm::mat3x4& mat);
        void uniform_matrix4x3fv(std::string_view name, const glm::mat4x3& mat);

        // floats uniforms
        inline void uniform(std::string_view name, const glm::vec1& v)
        {
            uniform1f(name, v.x);
        };

        inline void uniform(std::string_view name, const glm::vec2& v)
        {
            uniform2f(name, v.x, v.y);
        }

        inline void uniform(std::string_view name, const glm::vec3& v)
        {
            uniform3f(name, v.x, v.y, v.z);
        }

        inline void uniform(std::string_view name, const glm::vec4& v)
        {
            uniform4f(name, v.x, v.y, v.z, v.w);
        }

        // int uniforms
        inline void uniform(std::string_view name, const glm::ivec1& v)
        {
            uniform1i(name, v.x);
        };

        inline void uniform(std::string_view name, const glm::ivec2& v)
        {
            uniform2i(name, v.x, v.y);
        }

        inline void uniform(std::string_view name, const glm::ivec3& v)
        {
            uniform3i(name, v.x, v.y, v.z);
        }

        inline void uniform(std::string_view name, const glm::ivec4& v)
        {
            uniform4i(name, v.x, v.y, v.z, v.w);
        }

        // unsigned int uniforms
        inline void uniform(std::string_view name, const glm::uvec1& v)
        {
            uniform1i(name, v.x);
        };

        inline void uniform(std::string_view name, const glm::uvec2& v)
        {
            uniform2i(name, v.x, v.y);
        }

        inline void uniform(std::string_view name, const glm::uvec3& v)
        {
            uniform3i(name, v.x, v.y, v.z);
        }

        inline void uniform(std::string_view name, const glm::uvec4& v)
        {
            uniform4i(name, v.x, v.y, v.z, v.w);
        }

        // matrices uniforms
        inline void uniform(std::string_view name, const glm::mat2& mat)
        {
            uniform_matrix2fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat3& mat)
        {
            uniform_matrix3fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat4& mat)
        {
            uniform_matrix4fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat2x3& mat)
        {
            uniform_matrix2x3fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat3x2& mat)
        {
            uniform_matrix3x2fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat2x4& mat)
        {
            uniform_matrix2x4fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat4x2& mat)
        {
            uniform_matrix4x2fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat3x4& mat)
        {
            uniform_matrix3x4fv(name, mat);
        }

        inline void uniform(std::string_view name, const glm::mat4x3& mat)
        {
            uniform_matrix4x3fv(name, mat);
        }

        void save_uniform_location(std::string_view name);

    private:
        uint32_t get_uniform_location(std::string_view name);

        std::shared_ptr<shader_res> resource;

        std::unordered_map<std::string_view, uint32_t> uniform_locations;
    };
} // namespace Kengine::graphics