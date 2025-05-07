#pragma once

#include "Kengine/window/display.hxx"

#include "SDL3/SDL_video.h"

namespace Kengine::display
{
    bool initialize();
    bool get_closest_display_mode(mode d_mode, SDL_DisplayMode* sdl_mode);
} // namespace Kengine::display