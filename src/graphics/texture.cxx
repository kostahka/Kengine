#include "Kengine/graphics/texture.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    texture::texture(std::shared_ptr<texture_resource> resource)
        : resource(resource)
    {
        resource->take_data();
    }

    texture::~texture()
    {
        resource->free_data();
    }

    void texture::bind()
    {
        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, resource->get_id()));
    }
} // namespace Kengine::graphics