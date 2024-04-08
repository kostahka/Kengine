#pragma once

#include <cstdint>

namespace Kengine::audio
{
    typedef uint16_t format;

    struct spec
    {
        format fmt;
        int    channels;
        int    frequency;
    };
} // namespace Kengine::audio