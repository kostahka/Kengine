#include "Kengine/resources/font-resource.hxx"

#include "../io/file-manager.hxx"
#include "../opengl/opengl-debug.hxx"
#include "../scene/scene-manager.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/types/rect.hxx"

#include "imgui.h"

#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#define FONT_TEXTURE_RES 4096
#define MAX_GLYPH_SIZE   8

int closest_pow_2(int i)
{
    i--;
    int pi = 1;
    while (i > 0)
    {
        i >>= 1;
        pi <<= 1;
    }
    return pi;
}

int closest_pow_2_area(int i)
{
    i--;
    int pi = 1;
    while (i > 0)
    {
        i >>= 2;
        pi <<= 1;
    }
    return pi;
}

namespace Kengine
{
    font_resource::font_resource(std::string_view name)
        : texture_resource(name)
    {
        r_type = resource_type::font;
    }

    font_resource::font_resource(path font_path, std::string_view name)
        : texture_resource(name)
    {
        r_type = resource_type::font;
        f_path = font_path;
    }

    font_resource::~font_resource()
    {
        KENGINE_INFO("Unloaded font: {}", r_id.get_string());
    }

    std::size_t font_resource::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, font_size);
        size += serialization::write(os, first_char);
        size += serialization::write(os, last_char);
        size += serialization::write(os, mag_filter);
        size += serialization::write(os, min_filter);

        return size;
    }

    std::size_t font_resource::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, font_size);
        size += serialization::read(is, first_char);
        size += serialization::read(is, last_char);
        size += serialization::read(is, mag_filter);
        size += serialization::read(is, min_filter);

        return size;
    }

    std::size_t font_resource::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(font_size);
        size += serialization::size(first_char);
        size += serialization::size(last_char);
        size += serialization::size(mag_filter);
        size += serialization::size(min_filter);

        return size;
    }

    void font_resource::load_data()
    {
        if (!f_path.empty())
        {
            auto font_file = file_manager::load_file(
                scene_manager::assets_base_folder / f_path);
            if (font_file == nullptr)
            {
                load_invalid_texture();
                return;
            }

            file_manager::sdl_io_stream font_stream(font_file);
            TTF_Font* f = TTF_OpenFontIO(font_stream.get(), false, font_size);
            if (f == nullptr)
            {
                KENGINE_ERROR("Failed to open font [{}]",
                              f_path.string().c_str());
                load_invalid_texture();
                return;
            }

            if (last_char < first_char)
            {
                last_char = first_char;
            }

            size_t characters_count = last_char - first_char + 1;
            glyph_uvs      = std::make_unique<rect[]>(characters_count);
            glyph_advances = std::make_unique<float[]>(characters_count);
            glyphs         = std::make_unique<rect[]>(characters_count);

            uint16_t max_glyph_width  = 0;
            uint16_t max_glyph_height = 0;
            int      minx, maxx, miny, maxy, advance;
            for (auto ch = first_char; ch <= last_char; ++ch)
            {
                TTF_GetGlyphMetrics(
                    f, ch, &minx, &maxx, &miny, &maxy, &advance);
                int w                   = maxx - minx;
                int h                   = maxy - miny;
                glyphs[ch - first_char] = { (float)minx / (float)font_size,
                                            (float)miny / (float)font_size,
                                            (float)w / (float)font_size,
                                            (float)h / (float)font_size };
                if (w > max_glyph_width)
                {
                    max_glyph_width = w;
                }
                if (h > max_glyph_height)
                {
                    max_glyph_height = h;
                }
                glyph_advances[ch - first_char] =
                    (float)advance / (float)font_size;
            }
            space_advance = max_glyph_width;
            int max_area = static_cast<int>(max_glyph_width * max_glyph_height *
                                            characters_count);
            int texture_size = closest_pow_2_area(max_area);

            auto surface = SDL_CreateSurface(
                texture_size, texture_size, SDL_PIXELFORMAT_BGRA8888);

            auto pixel_format_details =
                SDL_GetPixelFormatDetails(surface->format);
            SDL_SetSurfaceColorKey(
                surface,
                true,
                SDL_MapRGBA(pixel_format_details, nullptr, 0, 0, 0, 0));

            SDL_Rect     dest{ 0, 0, 0, 0 };
            SDL_Surface* text;
            for (auto ch = first_char; ch <= last_char; ++ch)
            {
                text = TTF_RenderGlyph_Blended(
                    f, ch, SDL_Color{ 255, 255, 255, 255 });
                if (text)
                {
                    dest.w = text->w;
                    dest.h = text->h;

                    if (dest.x + dest.w >= texture_size)
                    {
                        dest.x = 0;
                        dest.y += dest.h + 1;

                        if (dest.y + dest.h >= texture_size)
                        {
                            load_invalid_texture();
                            KENGINE_ERROR(
                                "Out of glyph space in {}x{} font atlas "
                                "texture. Font: {}",
                                texture_size,
                                texture_size,
                                r_id.get_string());
                            glyph_uvs      = nullptr;
                            glyph_advances = nullptr;
                            SDL_DestroySurface(text);
                            SDL_DestroySurface(surface);
                            TTF_CloseFont(f);
                            return;
                        }
                    }

                    SDL_BlitSurface(text, nullptr, surface, &dest);
                    glyph_uvs[ch - first_char] = {
                        static_cast<float>(dest.x) / texture_size,
                        static_cast<float>(dest.y + dest.h) / texture_size,
                        static_cast<float>(dest.w) / texture_size,
                        static_cast<float>(-dest.h) / texture_size
                    };
                    SDL_DestroySurface(text);

                    dest.x += dest.w;
                }
                else
                {
                    glyph_uvs[ch - first_char] = { 0, 0, 1, 1 };
                }
            }

            KENGINE_GL_CHECK(glGenTextures(1, &id));
            KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, id));
            KENGINE_GL_CHECK(glTexImage2D(GL_TEXTURE_2D,
                                          0,
                                          GL_RGBA,
                                          texture_size,
                                          texture_size,
                                          0,
                                          GL_RGBA,
                                          GL_UNSIGNED_BYTE,
                                          surface->pixels));

            SDL_DestroySurface(surface);
            TTF_CloseFont(f);

            this->size = { texture_size, texture_size };

            KENGINE_INFO("Loaded font {}", r_id.get_string());

            GLint gl_min_filter = opengl::get_gl_texture_filter(min_filter);
            GLint gl_mag_filter = opengl::get_gl_texture_filter(mag_filter);

            KENGINE_GL_CHECK(glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
            KENGINE_GL_CHECK(glTexParameteri(
                GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));
        }
        else
        {
            load_invalid_texture();
            return;
        }

        GLint gl_min_filter = opengl::get_gl_texture_filter(min_filter);
        GLint gl_mag_filter = opengl::get_gl_texture_filter(mag_filter);

        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_min_filter));
        KENGINE_GL_CHECK(glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_mag_filter));
    }

    void font_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteTextures(1, &id));
            KENGINE_INFO("Unloaded font: {}", r_id.get_string());
            id = 0;
        }
    }

    static texture_format t_formats[]{
        texture_format::none,
        texture_format::red,
        texture_format::rg,
        texture_format::rgb,
        texture_format::rgba,
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

    bool font_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        edited = edited || imgui::edit_file("Font file", f_path);
        edited = edited || ImGui::DragInt("Font size", &font_size, 1, 0, 48);
        edited = edited || ImGui::DragInt("First char code",
                                          (int*)&first_char,
                                          1,
                                          0,
                                          static_cast<int>(0xFFFFFFFFFFFF));
        edited = edited || ImGui::DragInt("Last char code",
                                          (int*)&last_char,
                                          1,
                                          0,
                                          static_cast<int>(0xFFFFFFFFFFFF));

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

    rect font_resource::get_glyph(char32_t ch)
    {
        if (glyphs && ch >= first_char && ch <= last_char)
        {
            return glyphs[ch - first_char];
        }
        else if (first_char == 0)
        {
            return glyphs[first_char];
        }
        return { 0, 0, 0, 0 };
    }

    rect font_resource::get_glyph_uv(char32_t ch)
    {
        if (glyph_uvs && ch >= first_char && ch <= last_char)
        {
            return glyph_uvs[ch - first_char];
        }
        else if (first_char == 0)
        {
            return glyph_uvs[first_char];
        }
        return { 0, 0, 0, 0 };
    }

    float font_resource::get_glyph_advance(char32_t ch)
    {
        if (glyph_advances && ch >= first_char && ch <= last_char)
        {
            return glyph_advances[ch - first_char];
        }
        else if (first_char == 0)
        {
            return glyph_advances[first_char];
        }
        return space_advance;
    }
} // namespace Kengine