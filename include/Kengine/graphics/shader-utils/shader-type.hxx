#pragma once

#include "Kengine/units/matrix.hxx"
#include "Kengine/units/vector.hxx"

#include "glm/gtc/type_ptr.hpp"

#include <cstdint>
#include <string_view>
#include <tuple>


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
    struct shader_type_base_align;

    template <>
    struct shader_type_base_align<int> : std::integral_constant<uint32_t, 4>
    {
    };

    template <>
    struct shader_type_base_align<ivec2> : std::integral_constant<uint32_t, 8>
    {
    };

    template <>
    struct shader_type_base_align<ivec3> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<ivec4> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<unsigned int>
        : std::integral_constant<uint32_t, 4>
    {
    };

    template <>
    struct shader_type_base_align<uvec2> : std::integral_constant<uint32_t, 8>
    {
    };

    template <>
    struct shader_type_base_align<uvec3> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<uvec4> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<float> : std::integral_constant<uint32_t, 4>
    {
    };

    template <>
    struct shader_type_base_align<vec2> : std::integral_constant<uint32_t, 8>
    {
    };

    template <>
    struct shader_type_base_align<vec3> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<vec4> : std::integral_constant<uint32_t, 16>
    {
    };

    template <>
    struct shader_type_base_align<mat2x2> : std::integral_constant<uint32_t, 32>
    {
    };

    template <>
    struct shader_type_base_align<mat2x3> : std::integral_constant<uint32_t, 32>
    {
    };

    template <>
    struct shader_type_base_align<mat2x4> : std::integral_constant<uint32_t, 32>
    {
    };

    template <>
    struct shader_type_base_align<mat3x2> : std::integral_constant<uint32_t, 48>
    {
    };

    template <>
    struct shader_type_base_align<mat3x3> : std::integral_constant<uint32_t, 48>
    {
    };

    template <>
    struct shader_type_base_align<mat3x4> : std::integral_constant<uint32_t, 48>
    {
    };

    template <>
    struct shader_type_base_align<mat4x2> : std::integral_constant<uint32_t, 64>
    {
    };

    template <>
    struct shader_type_base_align<mat4x3> : std::integral_constant<uint32_t, 64>
    {
    };

    template <>
    struct shader_type_base_align<mat4x4> : std::integral_constant<uint32_t, 64>
    {
    };

    template <typename T>
    inline const void* get_shader_type_address(const T& value)
    {
        return &value;
    }

    inline const void* get_shader_type_address(const mat2x2& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat2x3& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat2x4& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat3x2& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat3x3& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat3x4& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat4x2& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat4x3& value)
    {
        return glm::value_ptr(value);
    }

    inline const void* get_shader_type_address(const mat4x4& value)
    {
        return glm::value_ptr(value);
    }

    namespace std140
    {
        template <typename T, typename... Types>
        struct layout;

        template <typename T>
        struct layout<T>
        {
            static constexpr uint32_t get(uint32_t  current_align,
                                          uint32_t* aligns)
            {
                constexpr uint32_t base_align =
                    shader_type_base_align<T>::value;
                constexpr uint32_t size = sizeof(T);

                if (current_align % base_align != 0)
                {
                    uint32_t next_align =
                        (current_align / base_align + 1) * base_align;
                    *aligns = next_align;
                    return next_align + size;
                }

                *aligns = current_align;
                return current_align + size;
            }
        };

        template <typename T, typename... Types>
        struct layout
        {
            static constexpr uint32_t get(uint32_t  current_align,
                                          uint32_t* aligns)
            {
                constexpr uint32_t base_align =
                    shader_type_base_align<T>::value;
                constexpr uint32_t size = sizeof(T);

                if (current_align % base_align != 0)
                {
                    uint32_t next_align =
                        (current_align / base_align + 1) * base_align;
                    *aligns = next_align;
                    return layout<Types...>::get(next_align + size, aligns + 1);
                }

                *aligns = current_align;
                return layout<Types...>::get(current_align + size, aligns + 1);
            }
        };

        template <typename T>
        constexpr void fill_data(char* data, uint32_t* aligns, const T& arg)
        {
            *(reinterpret_cast<T*>(data + *aligns)) = arg;
        }

        template <typename T, typename... Types>
        constexpr void fill_data(char*     data,
                                 uint32_t* aligns,
                                 const T&  arg,
                                 const Types&... args)
        {
            *(reinterpret_cast<T*>(data + *aligns)) = arg;
            fill_data(data, aligns + 1, args...);
        }

        template <typename... Types>
        constexpr std::pair<uint32_t, char*> get_buffer(uint32_t* aligns,
                                                        const Types&... args)
        {
            uint32_t total_size = layout<Types...>::get(0, aligns);

            char* data = new char[total_size];
            fill_data(data, aligns, args...);

            return std::pair(total_size, data);
        };

        template <int I, typename... Types>
        using type_at = std::tuple_element<I, std::tuple<Types...>>::type;

    }; // namespace std140

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