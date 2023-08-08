#include "Kengine/graphics/raw-vertex-buffer.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    const uint32_t g_byte   = GL_BYTE;
    const uint32_t g_ubyte  = GL_UNSIGNED_BYTE;
    const uint32_t g_short  = GL_SHORT;
    const uint32_t g_ushort = GL_UNSIGNED_SHORT;
    const uint32_t g_int    = GL_INT;
    const uint32_t g_uint   = GL_UNSIGNED_INT;
    const uint32_t g_float  = GL_FLOAT;
    const uint32_t g_double = GL_DOUBLE;

    raw_vertex_buffer::raw_vertex_buffer()
        : vbo(0)
        , valid(false)
    {
        KENGINE_GL_CHECK(glGenBuffers(1, &vbo));
    }

    raw_vertex_buffer::~raw_vertex_buffer()
    {
        if (vbo)
            KENGINE_GL_CHECK(glDeleteBuffers(1, &vbo));
    }

    void raw_vertex_buffer::allocate_data(void*    data,
                                          uint32_t size,
                                          bool     dynamic)
    {
        KENGINE_ASSERT(size != 0 && data, "No data to allocate");
        KENGINE_GL_CHECK(
            glBufferData(GL_ARRAY_BUFFER,
                         size,
                         data,
                         dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        valid = true;
    }

    void raw_vertex_buffer::replace_data(void*    data,
                                         uint32_t offset,
                                         uint32_t size)
    {
        KENGINE_GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
    }

    void raw_vertex_buffer::add_attribute_pointer(
        const vertex_attribute_pointer& attr_p)
    {
        attr_ptrs.push_back(attr_p);
    }

    void raw_vertex_buffer::bind()
    {
        KENGINE_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    }
} // namespace Kengine::graphics
