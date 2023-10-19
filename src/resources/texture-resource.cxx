#include "Kengine/resources/texture-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/log/log.hxx"

#include "picopng.hxx"

namespace Kengine
{
    texture_resource::texture_resource(path texture_path)
        : resource(resource_type::texture, texture_path)
    {
    }

    texture_resource::~texture_resource()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
        }
    }

    void texture_resource::load_data()
    {
        auto texture_file = file_manager::load_file(f_path);

        std::vector<unsigned char> image_memory;
        unsigned long              image_width  = 0;
        unsigned long              image_height = 0;

        if (decodePNG(image_memory,
                      image_width,
                      image_height,
                      reinterpret_cast<unsigned char*>(texture_file->rbegin()),
                      texture_file->size(),
                      true))
        {
            const char* str_path =
                reinterpret_cast<const char*>(f_path.c_str());
            KENGINE_ERROR("Failed to decode png texture from [{}]", str_path);
        }
        size = { static_cast<int>(image_width),
                 static_cast<int>(image_height) };

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA,
                     static_cast<GLsizei>(image_width),
                     static_cast<GLsizei>(image_height),
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     &image_memory[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void texture_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
        }
    }
} // namespace Kengine