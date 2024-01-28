#pragma once

#include "Kengine/units/vector.hxx"

#include <cstdint>
#include <vector>

namespace Kengine
{
    struct vertex
    {
        vec pos = { 0, 0, 0 };
    };

    struct vertex_color
    {
        vec  pos = { 0, 0, 0 };
        vec4 col = { 0, 0, 0, 0 };
    };

    struct vertex_text2d
    {
        vec  pos  = { 0, 0, 0 };
        vec2 text = { 0, 0 };
    };

    struct vertex_color_size
    {
        vec   pos  = { 0, 0, 0 };
        vec4  col  = { 0, 0, 0, 0 };
        float size = 0;
    };

    struct vertex_text2d_color
    {
        vec  pos  = { 0, 0, 0 };
        vec2 text = { 0, 0 };
        vec4 col  = { 0, 0, 0, 0 };
    };

    using vertex2d_array      = std::vector<vec2>;
    using vertex_array        = std::vector<vertex>;
    using vertex_color_array  = std::vector<vertex_color>;
    using vertex_text2d_array = std::vector<vertex_text2d>;
    using indexes_vector      = std::vector<uint32_t>;
} // namespace Kengine
