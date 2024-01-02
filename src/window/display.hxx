#pragma once

#include "SDL3/SDL_video.h"

namespace Kengine::display
{
    bool                   initialize();
    const SDL_DisplayMode* get_closest_display_mode(mode d_mode);
} // namespace Kengine::display