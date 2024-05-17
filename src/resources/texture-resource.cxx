#include "Kengine/resources/texture-resource.hxx"

#include "../opengl/opengl.hxx"
#include "../scene/scene-manager.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include "imgui.h"
#include "stb_image.h"

#include <iterator>
#include <unordered_map>

namespace Kengine
{
    texture_resource::texture_resource(std::string_view name)
        : resource(resource_type::texture, name)
        , id(0)
        , size({ 1, 1 })
        , num_channels(0)
        , min_filter(texture_filter::linear)
        , mag_filter(texture_filter::linear)
        , format(texture_format::rgba)
    {
    }

    texture_resource::texture_resource(path             texture_path,
                                       std::string_view name,
                                       texture_filter   mag_filter,
                                       texture_filter   min_filter)
        : resource(resource_type::texture, texture_path, name)
        , id(0)
        , size({ 0, 0 })
        , num_channels(0)
        , min_filter(min_filter)
        , mag_filter(mag_filter)
        , format(texture_format::none)
    {
    }

    texture_resource::texture_resource(ivec2            size,
                                       texture_format   format,
                                       std::string_view name,
                                       texture_filter   mag_filter,
                                       texture_filter   min_filter)
        : resource(resource_type::texture, name)
        , id(0)
        , size(size)
        , num_channels(0)
        , min_filter(min_filter)
        , mag_filter(mag_filter)
        , format(format)
    {
    }

    texture_resource::~texture_resource()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
            KENGINE_INFO("Unloaded texture: {}", id);
        }
    }

    void texture_resource::set_filter(texture_filter min_filter,
                                      texture_filter mag_filter)
    {
        this->min_filter = min_filter;
        this->mag_filter = mag_filter;

        GLint gl_min_filter = opengl::get_gl_texture_filter(min_filter);
        GLint gl_mag_filter = opengl::get_gl_texture_filter(mag_filter);

        if (id)
        {
            KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));

            KENGINE_GL_CHECK(glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
            KENGINE_GL_CHECK(glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));
        }
    }

    std::size_t texture_resource::serialize(std::ostream& os) const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();

        size += serialization::write(os, from_file);
        if (!from_file)
        {
            size += serialization::write(os, this->size.x);
            size += serialization::write(os, this->size.y);
            size += serialization::write(os, format);
        }
        size += serialization::write(os, mag_filter);
        size += serialization::write(os, min_filter);

        return size;
    }

    std::size_t texture_resource::deserialize(std::istream& is)
    {
        std::size_t size      = 0;
        bool        from_file = true;

        size += serialization::read(is, from_file);
        if (!from_file)
        {
            f_path.clear();
            size += serialization::read(is, this->size.x);
            size += serialization::read(is, this->size.y);
            size += serialization::read(is, format);
        }
        size += serialization::read(is, mag_filter);
        size += serialization::read(is, min_filter);

        return size;
    }

    std::size_t texture_resource::serialize_size() const
    {
        std::size_t size      = 0;
        const bool  from_file = !f_path.empty();

        size += serialization::size(from_file);
        if (!from_file)
        {
            size += serialization::size(this->size.x);
            size += serialization::size(this->size.y);
            size += serialization::size(format);
        }
        size += serialization::size(mag_filter);
        size += serialization::size(min_filter);

        return size;
    }

    void texture_resource::load_data()
    {
        if (!f_path.empty())
        {
            auto texture_file = file_manager::load_file(
                scene_manager::assets_base_folder / f_path);
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
                KENGINE_ERROR("Texture format is not supported. Num channels: "
                              "{}. File: {}",
                              num_channels,
                              f_path.string().c_str());
                load_invalid_texture();
                return;
            }

            format = opengl::get_texture_format_gl(dataFormat);

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
            stbi_image_free(image_memory);
        }
        else
        {
            KENGINE_GL_CHECK(glGenTextures(1, &id));
            KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));

            GLint gl_format = opengl::get_gl_texture_format(format);

            KENGINE_GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                                          0,
                                          gl_format,
                                          static_cast<GLsizei>(size.x),
                                          static_cast<GLsizei>(size.y),
                                          0,
                                          gl_format,
                                          GL_UNSIGNED_BYTE,
                                          nullptr));

            KENGINE_INFO("Loaded empty texture: {}", id);
        }

        GLint gl_min_filter = opengl::get_gl_texture_filter(min_filter);
        GLint gl_mag_filter = opengl::get_gl_texture_filter(mag_filter);

        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));
    }

    void texture_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
            KENGINE_INFO("Unloaded texture: {}", id);
            id = 0;
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

        format = texture_format::rgb;

        KENGINE_WARN("Unable to load texture, defaulting to checkerboard");
    }

    static texture_format t_formats[]{
        texture_format::none,
        texture_format::red,
        texture_format::rg,
        texture_format::rgb,
        texture_format::bgr,
        texture_format::rgba,
        texture_format::bgra,
        texture_format::depth,
        texture_format::depth_stencil,
    };

    static texture_filter t_filters[]{
        texture_filter::nearest,
        texture_filter::linear,
        texture_filter::nearest_mipmap_nearest,
        texture_filter::linear_mipmap_nearest,
        texture_filter::nearest_mipmap_linear,
        texture_filter::linear_mipmap_linear,
    };

    bool texture_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        edited = edited || imgui::edit_file("Texture file", f_path);

        if (f_path.empty())
        {
            edited = edited || ImGui::InputInt2("Size", (int*)&size);
        }

        auto current_min_filter = min_filter;
        auto current_min_filter_str =
            get_texture_filter_str(current_min_filter);
        if (ImGui::BeginCombo("Min filter", current_min_filter_str))
        {
            for (auto i = 0U; i != std::size(t_filters); ++i)
            {
                if (ImGui::Selectable(get_texture_filter_str(t_filters[i])))
                {
                    min_filter = t_filters[i];
                    edited     = true;
                }
            }
            ImGui::EndCombo();
        }

        auto current_mag_filter = mag_filter;
        auto current_mag_filter_str =
            get_texture_filter_str(current_mag_filter);
        if (ImGui::BeginCombo("Mag filter", current_mag_filter_str))
        {
            for (auto i = 0U; i != std::size(t_filters); ++i)
            {
                if (ImGui::Selectable(get_texture_filter_str(t_filters[i])))
                {
                    mag_filter = t_filters[i];
                    edited     = true;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::PopID();
#endif
        return edited;
    }

} // namespace Kengine