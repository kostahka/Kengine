#pragma once

#include "Kengine/graphics/draw-primitives.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{
    classSDL3TTF_VENDOREDgui_draw_debug
    {
    public:
        gui_draw_debug();
        ~gui_draw_debug();

        void create();
        void destroy();

        void draw(scene & sc);

    private:
        graphics::gl_render_primitive* r_points    = nullptr;
        graphics::gl_render_primitive* r_lines     = nullptr;
        graphics::gl_render_primitive* r_triangles = nullptr;
    };
} // namespace Kengine