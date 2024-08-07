#pragma once

#include "Kengine/types/vector.hxx"
#include "Kengine/window/window.hxx"

#include <SDL3/SDL_render.h>

#include <string_view>

struct SDL_Window;

namespace Kengine::window
{
    [[nodiscard]] bool initialize(std::string_view name);
    void               shutdown();

    extern SDL_Window*   window;
    extern SDL_GLContext context;

    extern bool gl_debug;

    extern int gl_major_version;
    extern int gl_minor_version;
    extern int gl_profile;

    void update_sizes();
    void update_flags();
} // namespace Kengine::window