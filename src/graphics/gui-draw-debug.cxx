#include "Kengine/graphics/gui-draw-debug.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/components/rect-transform-component.hxx"
#include "Kengine/graphics/graphics.hxx"

namespace Kengine
{
    gui_draw_debug::gui_draw_debug() {}

    gui_draw_debug::~gui_draw_debug() {}

    void gui_draw_debug::create()
    {
        r_points = Kengine::graphics::create_primitive_render(
            Kengine::graphics::primitive_type::points);
        r_points->create();
        r_lines = Kengine::graphics::create_primitive_render(
            Kengine::graphics::primitive_type::lines);
        r_lines->create();
        r_triangles = Kengine::graphics::create_primitive_render(
            Kengine::graphics::primitive_type::triangles);
        r_triangles->create();
    }

    void gui_draw_debug::destroy()
    {
        r_points->destroy();
        delete r_points;
        r_points = nullptr;

        r_lines->destroy();
        delete r_lines;
        r_lines = nullptr;

        r_triangles->destroy();
        delete r_triangles;
        r_triangles = nullptr;
    }

    void gui_draw_debug::draw(scene& sc)
    {
        constexpr float layer              = -1;
        constexpr vec4  rects_color        = { 0.5f, 0.5f, 0.5f, 0.5f };
        constexpr vec4  rects_line_color   = { 0.7f, 0.7f, 0.7f, 0.5f };
        constexpr vec4  anchor_line_color  = { 0.7f, 0.4f, 0.4f, 0.5f };
        constexpr vec4  anchor_point_color = { 0.9f, 0.1f, 0.1f, 0.5f };
        constexpr float anchor_size        = 10;

        auto current_viewport = graphics::get_current_viewport();
        auto projection       = glm::ortho(0.f,
                                     static_cast<float>(current_viewport.x),
                                     0.f,
                                     static_cast<float>(current_viewport.y));
        graphics::update_matrices(projection, mat4x4(1));

        auto rect_transform_view = sc.registry.view<rect_transform_component>();
        for (auto [ent, ent_trans] : rect_transform_view.each())
        {
            auto world_transform = ent_trans.get_world_transform();
            vec3 v1              = { (world_transform.start.x),
                                     (world_transform.start.y),
                                     layer };
            vec3 v2 = { (world_transform.start.x + world_transform.rect.x),
                        (world_transform.start.y),
                        layer };
            vec3 v3 = { (world_transform.start.x + world_transform.rect.x),
                        (world_transform.start.y + world_transform.rect.y),
                        layer };
            vec3 v4 = { (world_transform.start.x),
                        (world_transform.start.y + world_transform.rect.y),
                        layer };
            r_triangles->vertex(v1, rects_color);
            r_triangles->vertex(v2, rects_color);
            r_triangles->vertex(v3, rects_color);
            r_triangles->vertex(v3, rects_color);
            r_triangles->vertex(v4, rects_color);
            r_triangles->vertex(v1, rects_color);

            r_lines->vertex(v1, rects_line_color);
            r_lines->vertex(v2, rects_line_color);
            r_lines->vertex(v2, rects_line_color);
            r_lines->vertex(v3, rects_line_color);
            r_lines->vertex(v3, rects_line_color);
            r_lines->vertex(v4, rects_line_color);
            r_lines->vertex(v4, rects_line_color);
            r_lines->vertex(v1, rects_line_color);

            vec3 anc_v1 = { v1.x - ent_trans.transf.delta_min.x,
                            v1.y - ent_trans.transf.delta_min.y,
                            layer };
            vec3 anc_v2 = { v2.x - ent_trans.transf.delta_max.x,
                            v2.y - ent_trans.transf.delta_min.y,
                            layer };
            vec3 anc_v3 = { v3.x - ent_trans.transf.delta_max.x,
                            v3.y - ent_trans.transf.delta_max.y,
                            layer };
            vec3 anc_v4 = { v4.x - ent_trans.transf.delta_min.x,
                            v4.y - ent_trans.transf.delta_max.y,
                            layer };

            r_lines->vertex(anc_v1, anchor_line_color);
            r_lines->vertex(anc_v2, anchor_line_color);
            r_lines->vertex(anc_v2, anchor_line_color);
            r_lines->vertex(anc_v3, anchor_line_color);
            r_lines->vertex(anc_v3, anchor_line_color);
            r_lines->vertex(anc_v4, anchor_line_color);
            r_lines->vertex(anc_v4, anchor_line_color);
            r_lines->vertex(anc_v1, anchor_line_color);

            r_points->vertex(anc_v1, anchor_point_color, anchor_size);
            r_points->vertex(anc_v2, anchor_point_color, anchor_size);
            r_points->vertex(anc_v3, anchor_point_color, anchor_size);
            r_points->vertex(anc_v4, anchor_point_color, anchor_size);
        }

        r_triangles->draw();
        r_lines->draw();
        r_points->draw();
    }
} // namespace Kengine