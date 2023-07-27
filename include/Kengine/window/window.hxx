#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine::window
{
    struct options
    {
        ivec2 size{ 600, 400 };
        vec4  clear_color{ 0.0, 0.0, 0.0, 1.0 };

        int  gl_major_version{ 3 };
        int  gl_minor_version{ 0 };
        bool gl_profile_es{ true };
        bool gl_debug{ false };
    };

    void set_start_options(options&); // start window options

    ivec2 get_size_in_pixels();       // size of window in pixels
    ivec2 get_size();                 // size of window
    void  set_size(ivec2&);           // set window size
    void  set_fullscreen(bool);       // set fullscreen
    void  set_input_focus();          // set input focus on window
    void  raise();              // raise window and set input focus on window
    void  set_color(vec4& col); // set window area color
    void warp_mouse(float x, float y); // set mouse cursor at position of window
} // namespace Kengine::window