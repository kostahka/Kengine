#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/game.hxx"
#include "Kengine/units/vector.hxx"

#include <chrono>

typedef Kengine::game*    create_game_func();
extern "C" Kengine::game* create_game();

namespace Kengine
{
    // Engine configuration
    struct engine_configuration
    {
        // frequency of physics update
        std::chrono::duration<int, std::milli> update_delta_time;
        // frequency of render update
        std::chrono::duration<int, std::milli> render_delta_time;
    };

    [[nodiscard]] bool run(create_game_func* pf_create_game);

    void set_cursor_visible(bool visible);
    void draw_imgui();
    int  get_time_ms();

    void quit();
}; // namespace Kengine
