#include "Kengine/graphics/uniformbuffer.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    uniformbuffer::uniformbuffer()
    {
        KENGINE_GL_CHECK(glGenBuffers(1, &id));
    }

    uniformbuffer::~uniformbuffer()
    {
        KENGINE_GL_CHECK(glDeleteBuffers(1, &id));
        id = 0;
    }

    void uniformbuffer::bind()
    {
        KENGINE_GL_CHECK(glBindBuffer(GL_UNIFORM_BUFFER, id));
    }

    void uniformbuffer::uniform_data(const void* data,
                                     uint32_t    size,
                                     bool        dynamic)
    {
        KENGINE_GL_CHECK(
            glBufferData(GL_UNIFORM_BUFFER,
                         size,
                         data,
                         dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
    }

    void uniformbuffer::uniform_subdata(const void* data,
                                        uint32_t    size,
                                        uint32_t    offset)
    {
        KENGINE_GL_CHECK(
            glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    }

    void uniformbuffer::bind_end_point(uint32_t end_point)
    {
        KENGINE_GL_CHECK(glBindBufferBase(GL_UNIFORM_BUFFER, end_point, id));
    }
} // namespace Kengine::graphics