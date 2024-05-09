#pragma once

#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/system/system.hxx"
#include "Kengine/units/rect.hxx"

#include "entt/entt.hpp"

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace Kengine
{
    struct sprite_data
    {
        rect   uv;
        mat4x4 model;
    };

    class E_DECLSPEC graphics_system : public system
    {
    public:
        static constexpr auto name = "graphics_system";

        graphics_system(scene& sc);
        ~graphics_system();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        void on_start(scene&) override;

        void on_event(scene&, const event::game_event&) override;
        void on_render(scene&, int delta_ms) override;
        void on_update(scene&, int delta_ms) override;

    private:
        void sort_sprites(entt::registry&, entt::entity);
        void increase_data_size();
        void draw_sprites(uint32_t                           count,
                          res_ptr<sprite_material_resource>& material,
                          int                                layer);

        std::shared_ptr<graphics::vertex_buffer<sprite_data>> vbo;
        std::unique_ptr<graphics::vertex_element_array>       vao;
        std::vector<sprite_data>                              data;
        uint32_t max_sprites_count = 128;
        uint32_t vao_sprites_count = 128;

        bool update_projections = true;

        scene& sc;
    };
} // namespace Kengine