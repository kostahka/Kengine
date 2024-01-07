#pragma once

#include "Kengine/units/matrix.hxx"

namespace Kengine::graphics
{
    class camera
    {
    public:
        camera();
        ~camera();

        inline const mat4x4& get_projection() const { return projection; }

        inline const mat4x4& get_view() const { return view; };

        void set_matrices(mat4x4 projection, mat4x4 view);
        void set_projection(mat4x4 projection);
        void set_view(mat4x4 view);

        void bind();

    private:
        mat4x4 projection;
        mat4x4 view;

        bool binded;
    };
} // namespace Kengine::graphics