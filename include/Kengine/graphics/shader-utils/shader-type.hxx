#pragma once

#include "Kengine/units/matrix.hxx"
#include "Kengine/units/vector.hxx"

#include <cstdint>
#include <string_view>

namespace Kengine::graphics
{
    void set_uniform1i(uint32_t location, int value);
    void set_uniform2i(uint32_t location, ivec2 value);
    void set_uniform3i(uint32_t location, ivec3 value);
    void set_uniform4i(uint32_t location, ivec4 value);
    void set_uniform1ui(uint32_t location, unsigned int value);
    void set_uniform2ui(uint32_t location, uvec2 value);
    void set_uniform3ui(uint32_t location, uvec3 value);
    void set_uniform4ui(uint32_t location, uvec4 value);
    void set_uniform1f(uint32_t location, float value);
    void set_uniform2f(uint32_t location, vec2 value);
    void set_uniform3f(uint32_t location, vec3 value);
    void set_uniform4f(uint32_t location, vec4 value);
    void set_uniform_matrix2x2(uint32_t location, mat2x2 value);
    void set_uniform_matrix2x3(uint32_t location, mat2x3 value);
    void set_uniform_matrix2x4(uint32_t location, mat2x4 value);
    void set_uniform_matrix3x3(uint32_t location, mat3x3 value);
    void set_uniform_matrix3x2(uint32_t location, mat3x2 value);
    void set_uniform_matrix3x4(uint32_t location, mat3x4 value);
    void set_uniform_matrix4x4(uint32_t location, mat4x4 value);
    void set_uniform_matrix4x2(uint32_t location, mat4x2 value);
    void set_uniform_matrix4x3(uint32_t location, mat4x3 value);

    template <typename T>
    void set_uniform(uint32_t location, T value);

    template <typename T>
    struct shader_type
    {
    public:
        static constexpr uint32_t size       = sizeof(T);
        static constexpr uint32_t base_align = 0;
    };

    template <>
    struct shader_type<int>
    {
    public:
        static constexpr uint32_t size       = sizeof(int);
        static constexpr uint32_t base_align = 4;
    };

    template <>
    struct shader_type<ivec2>
    {
    public:
        static constexpr uint32_t size       = sizeof(ivec2);
        static constexpr uint32_t base_align = 8;
    };

    template <>
    struct shader_type<ivec3>
    {
    public:
        static constexpr uint32_t size       = sizeof(ivec3);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<ivec4>
    {
    public:
        static constexpr uint32_t size       = sizeof(ivec4);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<unsigned int>
    {
    public:
        static constexpr uint32_t size       = sizeof(unsigned int);
        static constexpr uint32_t base_align = 4;
    };

    template <>
    struct shader_type<uvec2>
    {
    public:
        static constexpr uint32_t size       = sizeof(uvec2);
        static constexpr uint32_t base_align = 8;
    };

    template <>
    struct shader_type<uvec3>
    {
    public:
        static constexpr uint32_t size       = sizeof(uvec3);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<uvec4>
    {
    public:
        static constexpr uint32_t size       = sizeof(uvec4);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<float>
    {
    public:
        static constexpr uint32_t size       = sizeof(float);
        static constexpr uint32_t base_align = 4;
    };

    template <>
    struct shader_type<vec2>
    {
    public:
        static constexpr uint32_t size       = sizeof(vec2);
        static constexpr uint32_t base_align = 8;
    };

    template <>
    struct shader_type<vec3>
    {
    public:
        static constexpr uint32_t size       = sizeof(vec3);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<vec4>
    {
    public:
        static constexpr uint32_t size       = sizeof(vec4);
        static constexpr uint32_t base_align = 16;
    };

    template <>
    struct shader_type<mat2x2>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat2x2);
        static constexpr uint32_t base_align = 32;
    };

    template <>
    struct shader_type<mat2x3>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat2x3);
        static constexpr uint32_t base_align = 32;
    };

    template <>
    struct shader_type<mat2x4>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat2x4);
        static constexpr uint32_t base_align = 32;
    };

    template <>
    struct shader_type<mat3x3>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat3x3);
        static constexpr uint32_t base_align = 48;
    };

    template <>
    struct shader_type<mat3x2>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat3x2);
        static constexpr uint32_t base_align = 48;
    };

    template <>
    struct shader_type<mat3x4>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat3x4);
        static constexpr uint32_t base_align = 48;
    };

    template <>
    struct shader_type<mat4x4>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat4x4);
        static constexpr uint32_t base_align = 64;
    };

    template <>
    struct shader_type<mat4x3>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat4x3);
        static constexpr uint32_t base_align = 64;
    };

    template <>
    struct shader_type<mat4x2>
    {
    public:
        static constexpr uint32_t size       = sizeof(mat4x2);
        static constexpr uint32_t base_align = 64;
    };

    template <>
    inline void set_uniform(uint32_t location, int value)

    {
        set_uniform1i(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, ivec2 value)
    {
        set_uniform2i(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, ivec3 value)
    {
        set_uniform3i(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, ivec4 value)
    {
        set_uniform4i(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, unsigned int value)
    {
        set_uniform1ui(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, uvec2 value)
    {
        set_uniform2ui(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, uvec3 value)
    {
        set_uniform3ui(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, uvec4 value)
    {
        set_uniform4ui(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, float value)
    {
        set_uniform1f(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, vec2 value)
    {
        set_uniform2f(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, vec3 value)
    {
        set_uniform3f(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, vec4 value)
    {
        set_uniform4f(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat2x2 value)
    {
        set_uniform_matrix2x2(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat2x3 value)
    {
        set_uniform_matrix2x3(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat2x4 value)
    {
        set_uniform_matrix2x4(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat3x3 value)
    {
        set_uniform_matrix3x3(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat3x2 value)
    {
        set_uniform_matrix3x2(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat3x4 value)
    {
        set_uniform_matrix3x4(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat4x4 value)
    {
        set_uniform_matrix4x4(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat4x2 value)
    {
        set_uniform_matrix4x2(location, value);
    }

    template <>
    inline void set_uniform(uint32_t location, mat4x3 value)
    {
        set_uniform_matrix4x3(location, value);
    }
} // namespace Kengine::graphics