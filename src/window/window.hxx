#pragma once

#include "Kengine/units/vector.hxx"

#include <string_view>

struct SDL_Window;
typedef void* SDL_GLContext;

namespace Kengine::window
{
    [[nodiscard]] bool initialize(std::string_view name = "");
    void               shutdown();

    extern SDL_Window*   window;
    extern SDL_GLContext context;

    extern bool gl_debug;

    extern int gl_major_version;
    extern int gl_minor_version;
    extern int gl_profile;

    void update_sizes();
} // namespace Kengine::window