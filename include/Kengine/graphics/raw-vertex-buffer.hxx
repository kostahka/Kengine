#pragma once

#include <cstdint>
#include <vector>

#include "Kengine/log/log.hxx"

namespace Kengine::graphics
{
    E_DECLSPEC extern const uint32_t g_byte;
    E_DECLSPEC extern const uint32_t g_ubyte;
    E_DECLSPEC extern const uint32_t g_short;
    E_DECLSPEC extern const uint32_t g_ushort;
    E_DECLSPEC extern const uint32_t g_int;
    E_DECLSPEC extern const uint32_t g_uint;
    E_DECLSPEC extern const uint32_t g_float;
    E_DECLSPEC extern const uint32_t g_double;

    struct vertex_attribute_pointer
    {
        vertex_attribute_pointer(uint32_t type,
                                 uint32_t count,
                                 uint32_t offset,
                                 uint32_t stride,
                                 uint32_t divisor = 0)
            : type(type)
            , count(count)
            , offset(offset)
            , stride(stride)
            , divisor(divisor)
        {
            // clang-format off
            KENGINE_ASSERT(type == g_byte   ||
                           type == g_ubyte  ||
                           type == g_short  ||
                           type == g_ushort ||
                           type == g_int    ||
                           type == g_uint   ||
                           type == g_float  ||
                           type == g_double,
                           "This type is not supported for attribute pointer");
            // clang-format on
        };

        uint32_t type;
        uint32_t count;
        uint32_t offset;
        uint32_t stride;
        uint32_t divisor;
    };

    class E_DECLSPEC raw_vertex_buffer
    {
    public:
        raw_vertex_buffer();
        ~raw_vertex_buffer();

        void allocate_data(void*    data,
                           uint32_t size,
                           bool     dynamic); // make sure do bind() before
        void replace_data(void*    data,
                          uint32_t offset,
                          uint32_t size); // make sure do bind() before

        void add_attribute_pointer(const vertex_attribute_pointer& attr_p);

        void bind();

        inline bool is_valid() const { return valid; };

        inline auto get_attr_ptrs() { return attr_ptrs; };

    private:
        uint32_t                              vbo;
        bool                                  valid;
        std::vector<vertex_attribute_pointer> attr_ptrs;
    };
} // namespace Kengine::graphics
