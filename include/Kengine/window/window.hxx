#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine::window
{
    ivec2 get_size_in_pixels(); // size of window in pixels
    ivec2 get_size();           // size of window
    void  set_size(ivec2);      // set window size
    void  set_fullscreen(bool); // set fullscreen
    void  set_input_focus();    // set input focus on window
    void  raise();              // raise window and set input focus on window
} // namespace Kengine::window