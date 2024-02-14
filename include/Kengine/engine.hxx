#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/game.hxx"
#include "Kengine/units/vector.hxx"

#include <chrono>



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

    [[nodiscard]] bool run(create_game_func* pf_create_game, const char* name);

    void set_cursor_visible(bool visible);
    int  get_time_ms();

    void quit();
}; // namespace Kengine
