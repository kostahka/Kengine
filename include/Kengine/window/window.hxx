#pragma once

#include "Kengine/configuration/configuration.hxx"
#include "Kengine/units/vector.hxx"

namespace Kengine::window
{
    void override_startup_options(
        const Kengine::configuration& override_configuration);

    [[nodiscard]] ivec2 get_size_in_pixels(); // size of window in pixels
    [[nodiscard]] ivec2 get_size();           // size of window
    [[nodiscard]] bool  is_maximized();
    [[nodiscard]] bool  is_hidden();

    void set_size(ivec2&);     // set window size
    void set_fullscreen(bool); // set fullscreen
    void set_input_focus();    // set input focus on window
    void raise();              // raise window and set input focus on window
    void warp_mouse(float x, float y); // set mouse cursor at position of window
} // namespace Kengine::window