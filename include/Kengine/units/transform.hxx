#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine
{
    struct transform
    {
        vec2  position = { 0, 0 };
        float angle    = 0;
        vec3  scale    = { 1, 1, 1 };
    };

    struct rect_transform
    {
        vec2 start{ 0, 0 };
        vec2 rect{ 1, 1 };
        bool enabled = true;
    };

    struct rect_data
    {
        vec2 anchor_min = { 0.5, 0.5 };
        vec2 anchor_max = { 0.5, 0.5 };
        vec2 delta_min  = { 0, 0 };
        vec2 delta_max  = { 0, 0 };
        bool enabled    = true;
    };
} // namespace Kengine
