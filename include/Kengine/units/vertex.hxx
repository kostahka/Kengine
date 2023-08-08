#pragma once

#include "Kengine/units/vector.hxx"

#include <cstdint>
#include <vector>

namespace Kengine
{
    struct vertex
    {
        vec pos;
    };

    struct vertex_color
    {
        vec  pos;
        vec4 col;
    };

    struct vertex_text2d
    {
        vec  pos;
        vec2 text;
    };

    struct vertex_color_size
    {
        vec   pos;
        vec4  col;
        float size;
    };

    struct vertex_text2d_color
    {
        vec  pos;
        vec2 text;
        vec4 col;
    };

    using vertex2d_array      = std::vector<vec2>;
    using vertex_array        = std::vector<vertex>;
    using vertex_color_array  = std::vector<vertex_color>;
    using vertex_text2d_array = std::vector<vertex_text2d>;
    using indexes_vector      = std::vector<uint32_t>;
} // namespace Kengine
