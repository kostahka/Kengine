
#pragma once

#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/system/system.hxx"
#include "Kengine/units/rect.hxx"

#include "entt/entt.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace Kengine
{
    struct gui_sprite_data
    {
        rect   uv;
        mat4x4 model;
    };

    class E_DECLSPEC gui_system : public system
    {
    public:
        static constexpr auto name = "gui_system";

        gui_system(scene& sc);
        ~gui_system();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        void on_event(scene&, const event::game_event&) override;
        void on_render(scene&, int delta_ms) override;
        // void on_update(scene&, int delta_ms) override;

    private:
        void sort_sprites(entt::registry&, entt::entity);
        void sort_text(entt::registry&, entt::entity);
        void increase_data_size();
        void draw_sprites(uint32_t                        count,
                          res_ptr<gui_material_resource>& material,
                          int                             layer);
        void draw_text_sprites(uint32_t                count,
                               res_ptr<font_resource>& font,
                               int                     layer,
                               vec4                    color);

        std::shared_ptr<graphics::vertex_buffer<gui_sprite_data>> vbo;
        std::unique_ptr<graphics::vertex_element_array>           vao;
        std::vector<gui_sprite_data>                              data;
        uint32_t              max_sprites_count = 128;
        uint32_t              vao_sprites_count = 128;
        std::vector<char32_t> text_word;
        std::vector<char32_t> text_line;

        scene& sc;
    };
} // namespace Kengine