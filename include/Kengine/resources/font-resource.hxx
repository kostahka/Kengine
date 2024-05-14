#pragma once

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/resources/resource-manager.hxx"
#include "Kengine/resources/texture-resource.hxx"
#include "Kengine/units/rect.hxx"
#include "Kengine/units/vector.hxx"
#include "resource.hxx"

#include <string>

namespace Kengine
{
    class E_DECLSPEC font_resource : public texture_resource
    {
    public:
        font_resource(std::string_view name);
        font_resource(std::filesystem::path f_path, std::string_view name);

        ~font_resource() override;

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

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
        int                      font_size      = 8;
        char32_t                 first_char     = 0;
        char32_t                 last_char      = 0;
        std::unique_ptr<rect[]>  glyphs         = nullptr;
        std::unique_ptr<rect[]>  glyph_uvs      = nullptr;
        std::unique_ptr<float[]> glyph_advances = nullptr;
        float                    space_advance  = 1;
    };
} // namespace Kengine