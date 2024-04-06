#pragma once

#include <cstdint>
#include <string_view>

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
