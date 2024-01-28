#include "Kengine/resources/renderbuffer-resource.hxx"

#include "../opengl/opengl.hxx"

namespace Kengine
{
    renderbuffer_resource::renderbuffer_resource(std::string_view name)
        : resource(resource_type::renderbuffer, name)
        , size(0, 0)
        , format(texture_internal_format::rgb8)
        , id(0)
    {
    }

    renderbuffer_resource::renderbuffer_resource(ivec2                   size,
                                                 texture_internal_format format,
                                                 std::string_view        name)
        : resource(resource_type::renderbuffer, name)
        , size(size)
        , format(format)
        , id(0)
    {
    }

    renderbuffer_resource::~renderbuffer_resource()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteRenderbuffers(1, &id));
            KENGINE_INFO("Unloaded renderbuffer: {}", id);
        }
    }

    std::size_t renderbuffer_resource::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, this->size.x);
        size += serialization::write(os, this->size.y);
        size += serialization::write(os, format);

        return size;
    }

    std::size_t renderbuffer_resource::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, this->size.x);
        size += serialization::read(is, this->size.y);
        size += serialization::read(is, format);

        return size;
    }

    void renderbuffer_resource::load_data()
    {
        KENGINE_GL_CHECK(glGenRenderbuffers(1, &id));
        KENGINE_GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, id));
        GLint gl_format = opengl::get_gl_texture_internal_format(format);
        KENGINE_GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER,
                                               gl_format,
                                               static_cast<GLsizei>(size.x),
                                               static_cast<GLsizei>(size.y)));
        KENGINE_INFO("Loaded renderbuffer: {}", id);
        KENGINE_GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }

    void renderbuffer_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteRenderbuffers(1, &id));
            KENGINE_INFO("Unloaded renderbuffer: {}", id);
            id = 0;
        }
    }
} // namespace Kengine