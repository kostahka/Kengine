#pragma once

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "Kengine/units/rect.hxx"
#include "Kengine/units/vector.hxx"
#include "resource.hxx"

#include <string>

namespace Kengine
{
    class E_DECLSPEC font_resource : public resource
    {
    public:
        font_resource(std::string_view name);
        font_resource(std::filesystem::path f_path, std::string_view name);

        ~font_resource() override;

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline const ivec2& get_texture_size() const
        {
            return texture_size;
        }

        [[nodiscard]] inline texture_filter get_min_filter() const
        {
            return min_filter;
        }

        [[nodiscard]] inline texture_filter get_mag_filter() const
        {
            return mag_filter;
        }

        inline rect* get_glyphs() { return glyphs.get(); }

        inline rect* get_glyph_uvs() { return glyph_uvs.get(); }

        inline float* get_glyph_advances() { return glyph_advances.get(); }

        rect  get_glyph(char32_t);
        rect  get_glyph_uv(char32_t);
        float get_glyph_advance(char32_t);

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        void load_invalid_texture();

        int                      font_size      = 8;
        char32_t                 first_char     = 0;
        char32_t                 last_char      = 0;
        std::unique_ptr<rect[]>  glyphs         = nullptr;
        std::unique_ptr<rect[]>  glyph_uvs      = nullptr;
        std::unique_ptr<float[]> glyph_advances = nullptr;
        float                    space_advance  = 1;

        ivec2          texture_size{ 1, 1 };
        uint32_t       id         = 0;
        texture_filter mag_filter = texture_filter::linear;
        texture_filter min_filter = texture_filter::linear;

        texture_format format = texture_format::rgba;
    };
} // namespace Kengine