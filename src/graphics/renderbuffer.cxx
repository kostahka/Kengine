#include "Kengine/graphics/renderbuffer.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    renderbuffer::renderbuffer(res_ptr<renderbuffer_resource> resource)
        : resource(resource)
    {
        resource->take_data();
    }

    renderbuffer::renderbuffer(const renderbuffer& other)
    {
        resource = other.resource;
        if (resource)
            resource->take_data();
    }

    renderbuffer::~renderbuffer()
    {
        if (resource)
            resource->free_data();
    }

    renderbuffer& renderbuffer::operator=(const renderbuffer& other)
    {
        if (resource)
            resource->free_data();

        resource = other.resource;
        if (resource)
            resource->take_data();

        return *this;
    }

    void renderbuffer::bind()
    {
        KENGINE_GL_CHECK(
            glBindRenderbuffer(GL_RENDERBUFFER, resource->get_id()));
    }

} // namespace Kengine::graphics