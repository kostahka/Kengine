#pragma once

#include "Kengine/graphics/shader-utils/shader-utils.hxx"
#include "Kengine/units/matrix.hxx"
#include "Kengine/units/vector.hxx"

namespace Kengine::graphics
{
    enum class shader_type
    {
        none,
        uint_type,
        int_type,
        float_type,
        vec2_type,
        vec3_type,
        vec4_type,
        ivec2_type,
        ivec3_type,
        ivec4_type,
        uvec2_type,
        uvec3_type,
        uvec4_type,
        mat2x2_type,
        mat2x3_type,
        mat2x4_type,
        mat3x2_type,
        mat3x3_type,
        mat3x4_type,
        mat4x2_type,
        mat4x3_type,
        mat4x4_type,
    };

    struct shader_type_any
    {
    public:
        shader_type_any()
            : type(shader_type::none)
        {
        }

        shader_type_any(unsigned int value)
            : type(shader_type::uint_type)
            , uint_value(value)
        {
        }

        shader_type_any(int value)
            : type(shader_type::int_type)
            , int_value(value)
        {
        }

        shader_type_any(float value)
            : type(shader_type::float_type)
            , float_value(value)
        {
        }

        shader_type_any(vec2 value)
            : type(shader_type::vec2_type)
            , vec2_value(value)
        {
        }

        shader_type_any(vec3 value)
            : type(shader_type::vec3_type)
            , vec3_value(value)
        {
        }

        shader_type_any(vec4 value)
            : type(shader_type::vec4_type)
            , vec4_value(value)
        {
        }

        shader_type_any(ivec2 value)
            : type(shader_type::ivec2_type)
            , ivec2_value(value)
        {
        }

        shader_type_any(ivec3 value)
            : type(shader_type::ivec3_type)
            , ivec3_value(value)
        {
        }

        shader_type_any(ivec4 value)
            : type(shader_type::ivec4_type)
            , ivec4_value(value)
        {
        }

        shader_type_any(uvec2 value)
            : type(shader_type::uvec2_type)
            , uvec2_value(value)
        {
        }

        shader_type_any(uvec3 value)
            : type(shader_type::uvec3_type)
            , uvec3_value(value)
        {
        }

        shader_type_any(uvec4 value)
            : type(shader_type::uvec4_type)
            , uvec4_value(value)
        {
        }

        shader_type_any(const mat2x2& value)
            : type(shader_type::mat2x2_type)
            , mat2x2_value(value)
        {
        }

        shader_type_any(const mat2x3& value)
            : type(shader_type::mat2x3_type)
            , mat2x3_value(value)
        {
        }

        shader_type_any(const mat2x4& value)
            : type(shader_type::mat2x4_type)
            , mat2x4_value(value)
        {
        }

        shader_type_any(const mat3x2& value)
            : type(shader_type::mat3x2_type)
            , mat3x2_value(value)
        {
        }

        shader_type_any(const mat3x3& value)
            : type(shader_type::mat3x3_type)
            , mat3x3_value(value)
        {
        }

        shader_type_any(const mat3x4& value)
            : type(shader_type::mat3x4_type)
            , mat3x4_value(value)
        {
        }

        shader_type_any(const mat4x2& value)
            : type(shader_type::mat4x2_type)
            , mat4x2_value(value)
        {
        }

        shader_type_any(const mat4x3& value)
            : type(shader_type::mat4x3_type)
            , mat4x3_value(value)
        {
        }

        shader_type_any(const mat4x4& value)
            : type(shader_type::mat4x4_type)
            , mat4x4_value(value)
        {
        }

        shader_type type;

        union
        {
            unsigned int uint_value;
            int          int_value;
            float        float_value;
            vec2         vec2_value;
            vec3         vec3_value;
            vec4         vec4_value;
            ivec2        ivec2_value;
            ivec3        ivec3_value;
            ivec4        ivec4_value;
            uvec2        uvec2_value;
            uvec3        uvec3_value;
            uvec4        uvec4_value;
            mat2x2       mat2x2_value;
            mat2x3       mat2x3_value;
            mat2x4       mat2x4_value;
            mat3x2       mat3x2_value;
            mat3x3       mat3x3_value;
            mat3x4       mat3x4_value;
            mat4x2       mat4x2_value;
            mat4x3       mat4x3_value;
            mat4x4       mat4x4_value;
        };

        inline void uniform(uint32_t location) const
        {
            switch (type)
            {
                case shader_type::none:
                    break;
                case shader_type::uint_type:
                    set_uniform(location, uint_value);
                    break;
                case shader_type::int_type:
                    set_uniform(location, int_value);
                    break;
                case shader_type::float_type:
                    set_uniform(location, float_value);
                    break;
                case shader_type::vec2_type:
                    set_uniform(location, vec2_value);
                    break;
                case shader_type::vec3_type:
                    set_uniform(location, vec3_value);
                    break;
                case shader_type::vec4_type:
                    set_uniform(location, vec4_value);
                    break;
                case shader_type::uvec2_type:
                    set_uniform(location, uvec2_value);
                    break;
                case shader_type::uvec3_type:
                    set_uniform(location, uvec3_value);
                    break;
                case shader_type::uvec4_type:
                    set_uniform(location, uvec4_value);
                    break;
                case shader_type::ivec2_type:
                    set_uniform(location, ivec2_value);
                    break;
                case shader_type::ivec3_type:
                    set_uniform(location, ivec3_value);
                    break;
                case shader_type::ivec4_type:
                    set_uniform(location, ivec4_value);
                    break;
                case shader_type::mat2x2_type:
                    set_uniform(location, mat2x2_value);
                    break;
                case shader_type::mat2x3_type:
                    set_uniform(location, mat2x3_value);
                    break;
                case shader_type::mat2x4_type:
                    set_uniform(location, mat2x4_value);
                    break;
                case shader_type::mat3x2_type:
                    set_uniform(location, mat3x2_value);
                    break;
                case shader_type::mat3x3_type:
                    set_uniform(location, mat3x3_value);
                    break;
                case shader_type::mat3x4_type:
                    set_uniform(location, mat3x4_value);
                    break;
                case shader_type::mat4x2_type:
                    set_uniform(location, mat4x2_value);
                    break;
                case shader_type::mat4x3_type:
                    set_uniform(location, mat4x3_value);
                    break;
                case shader_type::mat4x4_type:
                    set_uniform(location, mat4x4_value);
                    break;
            }
        }
    };
} // namespace Kengine::graphics