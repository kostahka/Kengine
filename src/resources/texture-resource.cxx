#include "Kengine/resources/texture-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/log/log.hxx"

#include "picopng.hxx"
#include "stb_image.h"

namespace Kengine
{
    GLint get_gl_texture_filter(texture_filter filter)
    {
        switch (filter)
        {
            case texture_filter::linear:
                return GL_LINEAR;
            case texture_filter::nearest:
                return GL_NEAREST;
            case texture_filter::nearest_mipmap_nearest:
                return GL_NEAREST_MIPMAP_NEAREST;
            case texture_filter::linear_mipmap_nearest:
                return GL_LINEAR_MIPMAP_NEAREST;
            case texture_filter::nearest_mipmap_linear:
                return GL_NEAREST_MIPMAP_LINEAR;
            case texture_filter::linear_mipmap_linear:
                return GL_LINEAR_MIPMAP_LINEAR;
            default:
                return 0;
        }
    }

    texture_resource::texture_resource(path           texture_path,
                                       texture_filter mag_filter,
                                       texture_filter min_filter)
        : resource(resource_type::texture, texture_path)
        , id(0)
        , size({ 0, 0 })
        , num_channels(0)
        , min_filter(min_filter)
        , mag_filter(mag_filter)
    {
    }

    texture_resource::~texture_resource()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
        }
    }

    void texture_resource::set_filter(texture_filter min_filter,
                                      texture_filter mag_filter)
    {
        this->min_filter = min_filter;
        this->mag_filter = mag_filter;

        GLint gl_min_filter = get_gl_texture_filter(min_filter);
        GLint gl_mag_filter = get_gl_texture_filter(mag_filter);

        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));

        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));
    }

    void texture_resource::load_data()
    {
        auto texture_file = file_manager::load_file(f_path);
        if (texture_file == nullptr)
        {
            load_invalid_texture();
            return;
        }

        unsigned char* image_memory;
        int            image_width        = 0;
        int            image_height       = 0;
        int            image_num_channels = 0;

        image_memory = stbi_load_from_memory(
            reinterpret_cast<unsigned char*>(texture_file->rbegin()),
            static_cast<int>(texture_file->size()),
            &image_width,
            &image_height,
            &image_num_channels,
            0);

        if (image_memory == nullptr)
        {
            KENGINE_ERROR("Failed to decode texture from [{}]",
                          f_path.string().c_str());
            load_invalid_texture();
            return;
        }
        size         = { image_width, image_height };
        num_channels = image_num_channels;

        GLenum dataFormat = 0;
        if (num_channels == 4)
        {
            dataFormat = GL_RGBA;
        }
        else if (num_channels == 3)
        {
            dataFormat = GL_RGB;
        }
        else
        {
            KENGINE_ERROR(
                "Texture format is not supported. Num channels: {}. File: {}",
                num_channels,
                f_path.string().c_str());
            load_invalid_texture();
            return;
        }

        KENGINE_GL_CHECK(glGenTextures(1, &id));
        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));

        KENGINE_GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                                      0,
                                      dataFormat,
                                      static_cast<GLsizei>(image_width),
                                      static_cast<GLsizei>(image_height),
                                      0,
                                      dataFormat,
                                      GL_UNSIGNED_BYTE,
                                      image_memory));

        KENGINE_INFO("Loaded {}-channel texture", num_channels);

        GLint gl_min_filter = get_gl_texture_filter(min_filter);
        GLint gl_mag_filter = get_gl_texture_filter(mag_filter);

        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));

        stbi_image_free(image_memory);
    }

    void texture_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
        }
    }

    void texture_resource::load_invalid_texture()
    {
        // clang-format off
        static const float pixels[] = {
            1.f, 0.f, 1.f,   0.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 0.f,
            0.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 0.f,   1.f, 0.f, 1.f,
            1.f, 0.f, 1.f,   0.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 0.f,
            0.f, 0.f, 0.f,   1.f, 0.f, 1.f,   0.f, 0.f, 0.f,   1.f, 0.f, 1.f,
        };
        // clang-format on

        size         = { 4, 4 };
        num_channels = 3;

        KENGINE_GL_CHECK(glGenTextures(1, &id));
        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));

        KENGINE_GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                                      0,
                                      GL_RGB,
                                      static_cast<GLsizei>(size.x),
                                      static_cast<GLsizei>(size.y),
                                      0,
                                      GL_RGB,
                                      GL_FLOAT,
                                      pixels));

        KENGINE_GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        KENGINE_GL_CHECK(
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        min_filter = texture_filter::nearest;
        mag_filter = texture_filter::nearest;

        KENGINE_WARN("Unable to load texture, defaulting to checkerboard");
    }

} // namespace Kengine