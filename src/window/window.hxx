#pragma once

#include "Kengine/units/vector.hxx"

#include <string_view>

struct SDL_Window;
typedef void *SDL_GLContext;

namespace Kengine::window
{
    bool initialize(std::string_view name = "", ivec2 size = { 0, 0 });
    void begin_render();
    void end_render();
    void shutdown();

    SDL_Window   *get_sdl_window();
    SDL_GLContext get_context();

    void update_sizes();
} // namespace Kengine::window