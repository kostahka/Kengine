#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/game.hxx"
#include "Kengine/units/vector.hxx"

#include <chrono>

namespace Kengine
{

    // game configuratiions
    struct game_configuration
    {
        // window sizes
        int screen_width;
        int screen_height;
        int pixels_width;
        int pixels_height;
    };

    // Engine configuration
    struct engine_configuration
    {
        // frequency of physics update
        std::chrono::duration<int, std::milli> update_delta_time;
        // frequency of render update
        std::chrono::duration<int, std::milli> render_delta_time;
    };

    std::string_view initialize();
    std::string_view shutdown();
    void             set_game(game *);
    std::string_view start_game_loop();

    void                                   set_cursor_visible(bool visible);
    void                                   clear_color(vec4);
    void                                   swap_buffers();
    void                                   draw_imgui();
    std::chrono::duration<int, std::milli> get_time();

    void quit();
}; // namespace Kengine

#ifdef ENGINE_DEV
extern "C" Kengine::game *create_game();
#endif