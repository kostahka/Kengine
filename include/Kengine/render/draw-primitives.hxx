#pragma once

#include "Kengine/units/vector.hxx"

namespace Kengine
{
    enum class primitive_type
    {
        points,
        lines,
        triangles
    };

    struct gl_render_primitive
    {
        virtual void create()                     = 0;
        virtual void destroy()                    = 0;
        virtual void vertex(const vec  &pos,
                            const vec4 &col,
                            const float size = 1) = 0;
        virtual void draw()                       = 0;

        virtual ~gl_render_primitive();
    };

    gl_render_primitive *create_primitive_render(primitive_type);
} // namespace Kengine