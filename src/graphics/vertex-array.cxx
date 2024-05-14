#include "Kengine/graphics/vertex-array.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    vertex_array::vertex_array()
        : vao(0)
        , valid(false)
        , attributes_count(0)
    {
        KENGINE_GL_CHECK(glGenVertexArrays(1, &vao));
    }

    vertex_array::~vertex_array()
    {
        vbos.clear();
        if (vao)
            KENGINE_GL_CHECK(glDeleteVertexArrays(1, &vao));
    }

    void vertex_array::add_vertex_buffer(std::shared_ptr<raw_vertex_buffer> vbo)
    {
        KENGINE_ASSERT(vbo->is_valid(), "Trying to bind not valid buffer");
        vbos.push_back(vbo);
        vbo->bind();
        for (auto& attr_ptr : vbo->get_attr_ptrs())
        {
            KENGINE_GL_CHECK(glVertexAttribPointer(
                attributes_count,
                attr_ptr.count,
                attr_ptr.type,
                GL_FALSE,
                attr_ptr.stride,
                reinterpret_cast<void*>(static_cast<size_t>(attr_ptr.offset))));

            if (attr_ptr.divisor)
            {
                KENGINE_GL_CHECK(
                    glVertexAttribDivisor(attributes_count, attr_ptr.divisor));
            }

            KENGINE_GL_CHECK(glEnableVertexAttribArray(attributes_count));

            attributes_count++;
        }
        valid = true;
    }

    void vertex_array::bind()
    {
        KENGINE_GL_CHECK(glBindVertexArray(vao));
    }

    void vertex_array::unbind()
    {
        KENGINE_GL_CHECK(glBindVertexArray(0));
    }

    void vertex_array::draw(draw_mode mode, uint32_t v_count, uint32_t v_start)
    {
        KENGINE_GL_CHECK(
            glDrawArrays(GL_POINTS + static_cast<int>(mode), v_start, v_count));
    }

    void vertex_array::draw_instanced(draw_mode mode,
                                      uint32_t  i_count,
                                      uint32_t  v_count,
                                      uint32_t  v_start)
    {
        KENGINE_GL_CHECK(glDrawArraysInstanced(
            GL_POINTS + static_cast<int>(mode), v_start, v_count, i_count));
    }

    vertex_element_array::vertex_element_array()
        : ebo(nullptr)
        , vertex_array()
    {
    }

    void vertex_element_array::set_elements(std::shared_ptr<element_buffer> ebo)
    {
        KENGINE_ASSERT(ebo->is_valid(), "Element buffer is not valid");
        this->ebo = ebo;
        ebo->bind();
    }

    void vertex_element_array::draw(draw_mode mode,
                                    uint32_t  v_count,
                                    uint32_t  v_start)
    {
        KENGINE_GL_CHECK(glDrawElements(
            GL_POINTS + static_cast<int>(mode),
            v_count,
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>(v_start * sizeof(uint32_t))));
    }

    void vertex_element_array::draw_instanced(draw_mode mode,
                                              uint32_t  i_count,
                                              uint32_t  v_count,
                                              uint32_t  v_start)
    {
        KENGINE_GL_CHECK(glDrawElementsInstanced(
            GL_POINTS + static_cast<int>(mode),
            v_count,
            GL_UNSIGNED_INT,
            reinterpret_cast<void*>(static_cast<size_t>(v_start) *
                                    sizeof(uint32_t)),
            i_count));
    }

} // namespace Kengine::graphics
