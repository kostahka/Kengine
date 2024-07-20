#pragma once

#include "Kengine/types/matrix.hxx"
#include "Kengine/types/rect.hxx"
#include "Kengine/types/transform.hxx"
#include "Kengine/types/vector.hxx"

#include <string>
#include <vector>

namespace Kengine
{
    enum any_type
    {
        t_uint,
        t_int,
        t_float,
        t_char,
        t_string,
        t_mat2x2,
        t_mat2x3,
        t_mat2x4,
        t_mat3x2,
        t_mat3x3,
        t_mat3x4,
        t_mat4x2,
        t_mat4x3,
        t_mat4x4,
        t_rect,
        t_irect,
        t_transform,
        t_rect_transform,
        t_rect_data,
        t_vec2,
        t_vec3,
        t_vec4,
        t_ivec2,
        t_ivec3,
        t_ivec4,
        t_uvec2,
        t_uvec3,
        t_uvec4,
        t_any_vector,
    };

    class any
    {
    public:
        any(unsigned int value)
        {
            uint_value = value;
            store_type = any_type::t_uint;
        }

        any(int value)
        {
            int_value  = value;
            store_type = any_type::t_int;
        }

        any(float value)
        {
            float_value = value;
            store_type  = any_type::t_float;
        }

        any(char value)
        {
            char_value = value;
            store_type = any_type::t_char;
        }

        any(const std::string& value)
        {
            str_value  = value;
            store_type = any_type::t_string;
        }

        any(const mat2x2& value)
        {
            mat2x2_value = value;
            store_type   = any_type::t_mat2x2;
        }

        any(const mat2x3& value)
        {
            mat2x3_value = value;
            store_type   = any_type::t_mat2x3;
        }

        any(const mat2x4& value)
        {
            mat2x4_value = value;
            store_type   = any_type::t_mat2x4;
        }

        any(const mat3x2& value)
        {
            mat3x2_value = value;
            store_type   = any_type::t_mat3x2;
        }

        any(const mat3x3& value)
        {
            mat3x3_value = value;
            store_type   = any_type::t_mat3x3;
        }

        any(const mat3x4& value)
        {
            mat3x4_value = value;
            store_type   = any_type::t_mat3x4;
        }

        any(const mat4x2& value)
        {
            mat4x2_value = value;
            store_type   = any_type::t_mat4x2;
        }

        any(const mat4x3& value)
        {
            mat4x3_value = value;
            store_type   = any_type::t_mat4x3;
        }

        any(const mat4x4& value)
        {
            mat4x4_value = value;
            store_type   = any_type::t_mat4x4;
        }

        any(const rect& value)
        {
            rect_value = value;
            store_type = any_type::t_rect;
        }

        any(const irect& value)
        {
            irect_value = value;
            store_type  = any_type::t_irect;
        }

        any(const transform& value)
        {
            transform_value = value;
            store_type      = any_type::t_transform;
        }

        any(const rect_transform& value)
        {
            rect_transform_value = value;
            store_type           = any_type::t_rect_transform;
        }

        any(const rect_data& value)
        {
            rect_data_value = value;
            store_type      = any_type::t_rect_data;
        }

        any(const vec2& value)
        {
            vec2_value = value;
            store_type = any_type::t_vec2;
        }

        any(const vec3& value)
        {
            vec3_value = value;
            store_type = any_type::t_vec3;
        }

        any(const vec4& value)
        {
            vec4_value = value;
            store_type = any_type::t_vec4;
        }

        any(const ivec2& value)
        {
            ivec2_value = value;
            store_type  = any_type::t_ivec2;
        }

        any(const ivec3& value)
        {
            ivec3_value = value;
            store_type  = any_type::t_ivec3;
        }

        any(const ivec4& value)
        {
            ivec4_value = value;
            store_type  = any_type::t_ivec4;
        }

        any(const uvec2& value)
        {
            uvec2_value = value;
            store_type  = any_type::t_uvec2;
        }

        any(const uvec3& value)
        {
            uvec3_value = value;
            store_type  = any_type::t_uvec3;
        }

        any(const uvec4& value)
        {
            uvec4_value = value;
            store_type  = any_type::t_uvec4;
        }

        any(const std::vector<any>& value)
        {
            any_vector_value = value;
            store_type       = any_type::t_any_vector;
        }

        any(any_type store_type)
            : store_type(store_type)
        {
            switch (store_type)
            {
                case any_type::t_int:
                    int_value = {};
                    break;
                case any_type::t_uint:
                    uint_value = {};
                    break;
                case any_type::t_float:
                    float_value = {};
                    break;
                case any_type::t_char:
                    char_value = {};
                    break;
                case any_type::t_string:
                    str_value = {};
                    break;
                case any_type::t_mat2x2:
                    mat2x2_value = {};
                    break;
                case any_type::t_mat2x3:
                    mat2x3_value = {};
                    break;
                case any_type::t_mat2x4:
                    mat2x4_value = {};
                    break;
                case any_type::t_mat3x2:
                    mat3x2_value = {};
                    break;
                case any_type::t_mat3x3:
                    mat3x3_value = {};
                    break;
                case any_type::t_mat3x4:
                    mat3x4_value = {};
                    break;
                case any_type::t_mat4x2:
                    mat4x2_value = {};
                    break;
                case any_type::t_mat4x3:
                    mat4x3_value = {};
                    break;
                case any_type::t_mat4x4:
                    mat4x4_value = {};
                    break;
                case any_type::t_rect:
                    rect_value = {};
                    break;
                case any_type::t_irect:
                    irect_value = {};
                    break;
                case any_type::t_transform:
                    transform_value = {};
                    break;
                case any_type::t_rect_transform:
                    rect_transform_value = {};
                    break;
                case any_type::t_rect_data:
                    rect_data_value = {};
                    break;
                case any_type::t_vec2:
                    vec2_value = {};
                    break;
                case any_type::t_vec3:
                    vec3_value = {};
                    break;
                case any_type::t_vec4:
                    vec4_value = {};
                    break;
                case any_type::t_ivec2:
                    ivec2_value = {};
                    break;
                case any_type::t_ivec3:
                    ivec3_value = {};
                    break;
                case any_type::t_ivec4:
                    ivec4_value = {};
                    break;
                case any_type::t_uvec2:
                    uvec2_value = {};
                    break;
                case any_type::t_uvec3:
                    uvec3_value = {};
                    break;
                case any_type::t_uvec4:
                    uvec4_value = {};
                    break;
                case any_type::t_any_vector:
                    any_vector_value = {};
                    break;
                default:
                    break;
            }
        }

        inline any_type m_type() const { return store_type; }

        inline unsigned int& m_unsigned_int() { return uint_value; }

        inline int& m_int() { return int_value; }

        inline float& m_float() { return float_value; }

        inline char& m_char() { return char_value; }

        inline std::string& m_string() { return str_value; }

        inline mat2x2& m_mat2x2() { return mat2x2_value; }

        inline mat2x3& m_mat2x3() { return mat2x3_value; }

        inline mat2x4& m_mat2x4() { return mat2x4_value; }

        inline mat3x2& m_mat3x2() { return mat3x2_value; }

        inline mat3x3& m_mat3x3() { return mat3x3_value; }

        inline mat3x4& m_mat3x4() { return mat3x4_value; }

        inline mat4x2& m_mat4x2() { return mat4x2_value; }

        inline mat4x3& m_mat4x3() { return mat4x3_value; }

        inline mat4x4& m_mat4x4() { return mat4x4_value; }

        inline rect& m_rect() { return rect_value; }

        inline irect& m_irect() { return irect_value; }

        inline transform& m_transform() { return transform_value; }

        inline rect_transform& m_rect_transform()
        {
            return rect_transform_value;
        }

        inline rect_data& m_rect_data() { return rect_data_value; }

        inline vec2& m_vec2() { return vec2_value; }

        inline vec3& m_vec3() { return vec3_value; }

        inline vec4& m_vec4() { return vec4_value; }

        inline ivec2& m_ivec2() { return ivec2_value; }

        inline ivec3& m_ivec3() { return ivec3_value; }

        inline ivec4& m_ivec4() { return ivec4_value; }

        inline uvec2& m_uvec2() { return uvec2_value; }

        inline uvec3& m_uvec3() { return uvec3_value; }

        inline uvec4& m_uvec4() { return uvec4_value; }

        inline std::vector<any>& m_any_vector() { return any_vector_value; }

        inline unsigned int get_unsigned_int(unsigned int default_value = {})
        {
            if (store_type == any_type::t_uint)
            {
                return uint_value;
            }
            return default_value;
        }

        inline int get_int(int default_value = {})
        {
            if (store_type == any_type::t_int)
            {
                return int_value;
            }
            return default_value;
        }

        inline float get_float(float default_value = {})
        {
            if (store_type == any_type::t_float)
            {
                return float_value;
            }
            return default_value;
        }

        inline char get_char(char default_value = {})
        {
            if (store_type == any_type::t_char)
            {
                return char_value;
            }
            return default_value;
        }

        inline std::string get_string(const std::string& default_value = {})
        {
            if (store_type == any_type::t_string)
            {
                return str_value;
            }
            return default_value;
        }

        inline mat2x2 get_mat2x2(const mat2x2& default_value = {})
        {
            if (store_type == any_type::t_mat2x2)
            {
                return mat2x2_value;
            }
            return default_value;
        }

        inline mat2x3 get_mat2x3(const mat2x3& default_value = {})
        {
            if (store_type == any_type::t_mat2x3)
            {
                return mat2x3_value;
            }
            return default_value;
        }

        inline mat2x4 get_mat2x4(const mat2x4& default_value = {})
        {
            if (store_type == any_type::t_mat2x4)
            {
                return mat2x4_value;
            }
            return default_value;
        }

        inline mat3x2 get_mat3x2(const mat3x2& default_value = {})
        {
            if (store_type == any_type::t_mat3x2)
            {
                return mat3x2_value;
            }
            return default_value;
        }

        inline mat3x3 get_mat3x3(const mat3x3& default_value = {})
        {
            if (store_type == any_type::t_mat3x3)
            {
                return mat3x3_value;
            }
            return default_value;
        }

        inline mat3x4 get_mat3x4(const mat3x4& default_value = {})
        {
            if (store_type == any_type::t_mat3x4)
            {
                return mat3x4_value;
            }
            return default_value;
        }

        inline mat4x2 get_mat4x2(const mat4x2& default_value = {})
        {
            if (store_type == any_type::t_mat4x2)
            {
                return mat4x2_value;
            }
            return default_value;
        }

        inline mat4x3 get_mat4x3(const mat4x3& default_value = {})
        {
            if (store_type == any_type::t_mat4x3)
            {
                return mat4x3_value;
            }
            return default_value;
        }

        inline mat4x4 get_mat4x4(const mat4x4& default_value = {})
        {
            if (store_type == any_type::t_mat4x4)
            {
                return mat4x4_value;
            }
            return default_value;
        }

        inline rect get_rect(const rect& default_value = {})
        {
            if (store_type == any_type::t_rect)
            {
                return rect_value;
            }
            return default_value;
        }

        inline irect get_irect(const irect& default_value = {})
        {
            if (store_type == any_type::t_irect)
            {
                return irect_value;
            }
            return default_value;
        }

        inline transform get_transform(const transform& default_value = {})
        {
            if (store_type == any_type::t_transform)
            {
                return transform_value;
            }
            return default_value;
        }

        inline rect_transform get_rect_transform(
            const rect_transform& default_value = {})
        {
            if (store_type == any_type::t_rect_transform)
            {
                return rect_transform_value;
            }
            return default_value;
        }

        inline rect_data get_rect_data(const rect_data& default_value = {})
        {
            if (store_type == any_type::t_rect_data)
            {
                return rect_data_value;
            }
            return default_value;
        }

        inline vec2 get_vec2(const vec2& default_value = {})
        {
            if (store_type == any_type::t_vec2)
            {
                return vec2_value;
            }
            return default_value;
        }

        inline vec3 get_vec3(const vec3& default_value = {})
        {
            if (store_type == any_type::t_vec3)
            {
                return vec3_value;
            }
            return default_value;
        }

        inline vec4 get_vec4(const vec4& default_value = {})
        {
            if (store_type == any_type::t_vec4)
            {
                return vec4_value;
            }
            return default_value;
        }

        inline ivec2 get_ivec2(const ivec2& default_value = {})
        {
            if (store_type == any_type::t_ivec2)
            {
                return ivec2_value;
            }
            return default_value;
        }

        inline ivec3 get_ivec3(const ivec3& default_value = {})
        {
            if (store_type == any_type::t_ivec3)
            {
                return ivec3_value;
            }
            return default_value;
        }

        inline ivec4 get_ivec4(const ivec4& default_value = {})
        {
            if (store_type == any_type::t_ivec4)
            {
                return ivec4_value;
            }
            return default_value;
        }

        inline uvec2 get_uvec2(const uvec2& default_value = {})
        {
            if (store_type == any_type::t_uvec2)
            {
                return uvec2_value;
            }
            return default_value;
        }

        inline uvec3 get_uvec3(const uvec3& default_value = {})
        {
            if (store_type == any_type::t_uvec3)
            {
                return uvec3_value;
            }
            return default_value;
        }

        inline uvec4 get_uvec4(const uvec4& default_value = {})
        {
            if (store_type == any_type::t_uvec4)
            {
                return uvec4_value;
            }
            return default_value;
        }

        inline std::vector<any> get_any_vector(
            const std::vector<any>& default_value = {})
        {
            if (store_type == any_type::t_uvec4)
            {
                return any_vector_value;
            }
            return default_value;
        }

    private:
        any_type store_type;

        union
        {
            unsigned int     uint_value;
            int              int_value;
            float            float_value;
            char             char_value;
            std::string      str_value;
            mat2x2           mat2x2_value;
            mat2x3           mat2x3_value;
            mat2x4           mat2x4_value;
            mat3x2           mat3x2_value;
            mat3x3           mat3x3_value;
            mat3x4           mat3x4_value;
            mat4x2           mat4x2_value;
            mat4x3           mat4x3_value;
            mat4x4           mat4x4_value;
            rect             rect_value;
            irect            irect_value;
            transform        transform_value;
            rect_transform   rect_transform_value;
            rect_data        rect_data_value;
            vec2             vec2_value;
            vec3             vec3_value;
            vec4             vec4_value;
            ivec2            ivec2_value;
            ivec3            ivec3_value;
            ivec4            ivec4_value;
            uvec2            uvec2_value;
            uvec3            uvec3_value;
            uvec4            uvec4_value;
            std::vector<any> any_vector_value;
        };
    };
} // namespace Kengine