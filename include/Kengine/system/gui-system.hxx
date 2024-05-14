
#pragma once

#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"
#include "Kengine/resources/font-material-resource.hxx"
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

    struct text_sprite_data
    {
        rect   uv;
        vec4   color;
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
        void increase_gui_data_size();
        void increase_text_data_size();
        void draw_sprites(uint32_t                        count,
                          res_ptr<gui_material_resource>& material,
                          int                             layer);
        void draw_text_sprites(uint32_t                         count,
                               res_ptr<font_material_resource>& material,
                               int                              layer);
        void create_gui_vao();
        void create_text_vao();

        std::vector<std::shared_ptr<graphics::vertex_buffer<gui_sprite_data>>>
            gui_vbos{};
        std::vector<std::shared_ptr<graphics::vertex_buffer<text_sprite_data>>>
            text_vbos{};
        std::vector<std::shared_ptr<graphics::vertex_element_array>> gui_vaos{};
        std::vector<std::shared_ptr<graphics::vertex_element_array>>
                                      text_vaos{};
        std::vector<gui_sprite_data>  gui_data{};
        std::vector<text_sprite_data> text_data{};
        std::vector<char32_t>         text_word{};
        std::vector<char32_t>         text_line{};
        uint32_t                      gui_vao_number  = 0;
        uint32_t                      text_vao_number = 0;

        std::shared_ptr<graphics::vertex_buffer<vec3>> sprite_vbo = nullptr;
        std::shared_ptr<graphics::element_buffer>      sprite_ebo = nullptr;

        scene& sc;
    };
} // namespace Kengine