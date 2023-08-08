#include "Kengine/graphics/element-buffer.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    element_buffer::element_buffer()
        : ebo(0)
        , indexes_count(0)
    {
        KENGINE_GL_CHECK(glGenBuffers(1, &ebo));
    }

    element_buffer::~element_buffer()
    {
        if (ebo)
            KENGINE_GL_CHECK(glDeleteBuffers(1, &ebo));
    }

    void element_buffer::bind()
    {
        KENGINE_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    }

    void element_buffer::allocate_indexes(uint32_t* indexes,
                                          uint32_t  count,
                                          bool      dynamic)
    {
        KENGINE_ASSERT(indexes && count, "No data to allocate");
        KENGINE_GL_CHECK(
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         count * sizeof(uint32_t),
                         indexes,
                         dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        valid = true;
    }

    void element_buffer::set_indexes(uint32_t* indexes,
                                     uint32_t  offset,
                                     uint32_t  count)
    {
        KENGINE_GL_CHECK(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                                         offset * sizeof(uint32_t),
                                         count * sizeof(uint32_t),
                                         indexes));
    }
} // namespace Kengine::graphics