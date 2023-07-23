#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine
{
    struct transform
    {
        vec  pos;
        vec2 angle;
    };

    struct transform2
    {
        vec2  pos;
        float angle;
    };

    struct itransform
    {
        ivec pos;
        vec2 angle;
    };

    struct itransform2
    {
        ivec2 pos;
        float angle;
    };

} // namespace Kengine
