#include "Kengine/graphics/camera.hxx"

#include "render-manager.hxx"

namespace Kengine::graphics
{
    static camera* current_camera = nullptr;

    camera::camera()
        : projection(1)
        , view(1)
        , binded(false)
    {
    }

    camera::~camera()
    {
        if (binded)
        {
            current_camera = nullptr;
            Kengine::graphics::render_manager::update_matrices(mat4x4(1),
                                                               mat4x4(1));
        }
    }

    void camera::set_matrices(mat4x4 projection, mat4x4 view)
    {
        this->projection = projection;
        this->view       = view;
        if (binded)
        {
            render_manager::update_matrices(projection, view);
        }
    }

    void camera::set_projection(mat4x4 projection)
    {
        this->projection = projection;
        if (binded)
        {
            render_manager::update_projection(projection);
        }
    }

    void camera::set_view(mat4x4 view)
    {
        this->view = view;
        if (binded)
        {
            render_manager::update_view(view);
        }
    }

    void camera::bind()
    {
        if (current_camera)
        {
            current_camera->binded = false;
        }

        current_camera = this;
        binded         = true;

        render_manager::update_matrices(projection, view);
    }
} // namespace Kengine::graphics