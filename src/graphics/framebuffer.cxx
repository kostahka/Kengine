#include "Kengine/graphics/framebuffer.hxx"

#include "../opengl/opengl.hxx"

#include "Kengine/graphics/render-manager.hxx"

namespace Kengine::graphics
{
    framebuffer::framebuffer()
        : resource(nullptr)
        , clear_color(0)
    {
    }

    framebuffer::framebuffer(res_ptr<framebuffer_resource> resource,
                             vec4                          clear_color)
        : resource(resource)
        , clear_color(clear_color)
    {
        resource->take_data();
    }

    framebuffer::framebuffer(const framebuffer& other)
    {
        resource    = other.resource;
        clear_color = other.clear_color;
        if (resource)
            resource->take_data();
    }

    framebuffer::~framebuffer()
    {
        if (resource)
            resource->free_data();
    }

    framebuffer& framebuffer::operator=(const framebuffer& other)
    {
        if (resource)
            resource->free_data();

        resource    = other.resource;
        clear_color = other.clear_color;
        if (resource)
            resource->take_data();

        return *this;
    }

    void framebuffer::clear()
    {
        render_manager::set_clear_color(clear_color);
        // For test
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                                 GL_STENCIL_BUFFER_BIT));
    }

    void framebuffer::set_clear_color(vec4 color)
    {
        clear_color = color;
    }
} // namespace Kengine::graphics