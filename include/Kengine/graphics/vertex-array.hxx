#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "Kengine/graphics/element-buffer.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"

namespace Kengine::graphics
{
    enum class draw_mode
    {
        points         = 0,
        lines          = 1,
        line_loop      = 2,
        line_strip     = 3,
        triangles      = 4,
        triangle_strip = 5,
        triangle_fan   = 6
    };

    class vertex_array
    {
    public:
        vertex_array();
        ~vertex_array();

        void add_vertex_buffer(std::shared_ptr<raw_vertex_buffer>
                                   vbo); // make sure do bind() before

        void bind();

        void draw(draw_mode mode,
                  uint32_t  v_count,
                  uint32_t  v_start = 0); // make sure do bind() before
        void draw_instanced(draw_mode mode,
                            uint32_t  i_count,
                            uint32_t  v_count,
                            uint32_t v_start = 0); // make sure do bind() before

    protected:
        uint32_t vao;
        uint32_t attributes_count;

        std::vector<std::shared_ptr<raw_vertex_buffer>> vbos;
        bool                                            valid;
    };

    class vertex_element_array : public vertex_array
    {
    public:
        vertex_element_array();

        void bind();

        void set_elements(
            std::shared_ptr<element_buffer> ebo); // make sure do bind() before

        void draw(draw_mode mode,
                  uint32_t  v_count,
                  uint32_t  v_start = 0); // make sure do bind() before
        void draw_instanced(draw_mode mode,
                            uint32_t  i_count,
                            uint32_t  v_count,
                            uint32_t v_start = 0); // make sure do bind() before

    private:
        std::shared_ptr<element_buffer> ebo;
    };
} // namespace Kengine::graphics