#pragma once

#include "Kengine/units/vector.hxx"

#include <SDL3/SDL_audio.h>

namespace Kengine
{
    int mix_audio_format_two_channel(Uint8*          dst,
                                     const Uint8*    src,
                                     SDL_AudioFormat format,
                                     Uint32          len,
                                     vec2            volume);
}