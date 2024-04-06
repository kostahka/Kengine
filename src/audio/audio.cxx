#include "audio.hxx"

#include <algorithm>
#include <map>
#include <mutex>

#include "Kengine/audio/audio.hxx"
#include "Kengine/log/log.hxx"

#include "SDL3/SDL_audio.h"

namespace Kengine::audio
{

    std::string_view get_sound_format_name(uint16_t format_value)
    {
        static const std::map<int, std::string_view> format = {
            {SDL_AUDIO_U8,     "AUDIO_U8"   },
            { SDL_AUDIO_S8,    "AUDIO_S8"   },
            { SDL_AUDIO_S16LE, "AUDIO_S16LE"},
            { SDL_AUDIO_S16BE, "AUDIO_S16BE"},
            { SDL_AUDIO_S32LE, "AUDIO_S32LE"},
            { SDL_AUDIO_S32BE, "AUDIO_S32BE"},
            { SDL_AUDIO_F32LE, "AUDIO_F32LE"},
            { SDL_AUDIO_F32BE, "AUDIO_F32BE"},
        };

        auto it = format.find(format_value);
        return it->second;
    }

    std::size_t get_sound_format_size(uint16_t format_value)
    {
        static const std::map<int, std::size_t> format = {
            {SDL_AUDIO_U8,     1},
            { SDL_AUDIO_S8,    1},
            { SDL_AUDIO_S16LE, 2},
            { SDL_AUDIO_S16BE, 2},
            { SDL_AUDIO_S32LE, 4},
            { SDL_AUDIO_S32BE, 4},
            { SDL_AUDIO_F32LE, 4},
            { SDL_AUDIO_F32BE, 4},
        };

        auto it = format.find(format_value);
        return it->second;
    }

} // namespace Kengine::audio
