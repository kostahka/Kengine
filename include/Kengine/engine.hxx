#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/game.hxx"
#include "Kengine/types/vector.hxx"

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

    [[nodiscard]] E_DECLSPEC bool run(create_game_func* pf_create_game,
                                      const char*       name);

    E_DECLSPEC void set_cursor_visible(bool visible);
    E_DECLSPEC int  get_time_ms();

    E_DECLSPEC void quit();
}; // namespace Kengine
