#include "Kengine/graphics/framebuffer.hxx"

#include "../opengl/opengl.hxx"

#include "Kengine/graphics/render-manager.hxx"

namespace Kengine::graphics
{
    framebuffer::framebuffer()
        : resource(nullptr)
    {
    }

    framebuffer::framebuffer(res_ptr<framebuffer_resource> resource)
        : resource(resource)
    {
        resource->take_data();
    }

    framebuffer::framebuffer(const framebuffer& other)
    {
        resource = other.resource;
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

        resource = other.resource;
        if (resource)
            resource->take_data();

        return *this;
    }

    void framebuffer::clear()
    {
        auto clear_color = resource->get_clear_color();
        KENGINE_GL_CHECK(glClearColor(
            clear_color.r, clear_color.g, clear_color.b, clear_color.a));
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                                 GL_STENCIL_BUFFER_BIT));
    }

    void framebuffer::set_clear_color(const vec4& color)
    {
        resource->set_clear_color(color);
    }
} // namespace Kengine::graphics