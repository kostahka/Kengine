#pragma once

#include "Kengine/configuration/configuration.hxx"
#include "Kengine/types/vector.hxx"

namespace Kengine::window
{
    E_DECLSPEC void override_startup_options(
        const Kengine::configuration& override_configuration);

    [[nodiscard]] E_DECLSPEC ivec2
    get_size_in_pixels();                      // size of window in pixels
    [[nodiscard]] E_DECLSPEC ivec2 get_size(); // size of window
    [[nodiscard]] E_DECLSPEC bool  is_maximized();
    [[nodiscard]] E_DECLSPEC bool  is_hidden();

    E_DECLSPEC void set_size(ivec2&);     // set window size
    E_DECLSPEC void set_fullscreen(bool); // set fullscreen
    E_DECLSPEC void set_input_focus();    // set input focus on window
    E_DECLSPEC void raise(); // raise window and set input focus on window
    E_DECLSPEC void warp_mouse(
        float x, float y); // set mouse cursor at position of window
} // namespace Kengine::window