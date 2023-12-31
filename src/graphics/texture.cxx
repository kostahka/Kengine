#include "Kengine/graphics/texture.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine::graphics
{
    texture::texture(res_ptr<texture_resource> resource)
        : resource(resource)
    {
        resource->take_data();
    }

    texture::texture(const texture& other)
    {
        resource = other.resource;
        if (resource)
            resource->take_data();
    }

    texture::~texture()
    {
        if (resource)
            resource->free_data();
    }

    texture& texture::operator=(const texture& other)
    {
        if (resource)
            resource->free_data();

        resource = other.resource;
        if (resource)
            resource->take_data();

        return *this;
    }

    void texture::bind(uint32_t texture_block)
    {
        KENGINE_ASSERT(texture_block < 32,
                       "Texture block id must be less than 32");
        KENGINE_GL_CHECK(glActiveTexture(GL_TEXTURE0 + texture_block));
        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, resource->get_id()));
    }
} // namespace Kengine::graphics