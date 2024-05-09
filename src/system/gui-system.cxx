#include "Kengine/system/gui-system.hxx"

#include "../event/event.hxx"
#include "../graphics/graphics.hxx"
#include "Kengine/components/gui-component.hxx"
#include "Kengine/components/rect-transform-component.hxx"
#include "Kengine/components/text-component.hxx"
#include "Kengine/event/event.hxx"
#include "Kengine/io/input.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{
    static string_id text_color_property_id;

    size_t unicode_strlen(const std::u32string& u_str)
    {
        const char* str = (const char*)u_str.data();
        int         c, i, ix, q;
        for (q = 0, i = 0, ix = static_cast<int>(u_str.length()); i < ix;
             i++, q++)
        {
            c = (unsigned char)str[i];
            if (c >= 0 && c <= 127)
                i += 0;
            else if ((c & 0xE0) == 0xC0)
                i += 1;
            else if ((c & 0xF0) == 0xE0)
                i += 2;
            else if ((c & 0xF8) == 0xF0)
                i += 3;
            else
                return 0;
        }
        return q;
    }

    static char32_t next_glyph(const char32_t* u_str, int* i)
    {
        const char* str = reinterpret_cast<const char*>(u_str);
        int         len;
        const char* p;

        unsigned int bit = (unsigned char)str[*i];

        if (bit >= 0 && bit <= 127)
        {
            len = 1;
        }
        else if ((bit & 0xE0) == 0xC0)
        {
            len = 2;
        }
        else if ((bit & 0xF0) == 0xE0)
        {
            len = 3;
        }
        else if ((bit & 0xF8) == 0xF0)
        {
            len = 4;
        }
        else
        {
            len = 0;
        }

        char32_t glyph;
        p = str + *i;

        memset(&glyph, 0, sizeof(glyph));

        memcpy(&glyph, p, len);

        *i = *i + len;

        return glyph;
    }

    static float text_line_width(const char32_t*         u_str,
                                 res_ptr<font_resource>& font)
    {
        float    width = 0;
        int      i     = 0;
        char32_t glyph = 0;
        while ((glyph = next_glyph(u_str, &i)) && (glyph != U'\n'))
        {
            width += font->get_glyph_advance(glyph);
        }
        return width;
    }

    gui_system::gui_system(scene& sc)
        : system(name, system_event_type | system_render_type)
        , sc(sc)
    {
        text_color_property_id = hash_string("color");
        vao = std::make_unique<graphics::vertex_element_array>();
        vao->bind();

        auto sprite_vbo = std::make_shared<graphics::vertex_buffer<vec3>>();

        static std::vector<vec3> sprite_pos_vertices{
            {0,  0, 0},
            { 0, 1, 0},
            { 1, 1, 0},
            { 1, 0, 0},
        };

        sprite_vbo->bind();
        sprite_vbo->allocate_vertices(sprite_pos_vertices.data(), 4, false);
        sprite_vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float, 3, 0, sizeof(vec3)));

        vao->add_vertex_buffer(sprite_vbo);

        vbo = std::make_shared<graphics::vertex_buffer<gui_sprite_data>>();

        data.resize(max_sprites_count);

        vbo->bind();

        vbo->allocate_vertices(data.data(), max_sprites_count, true);
        vbo->add_attribute_pointer(
            graphics::vertex_attribute_pointer(graphics::g_float,
                                               2,
                                               offsetof(gui_sprite_data, uv),
                                               sizeof(gui_sprite_data),
                                               1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            2,
            offsetof(gui_sprite_data, uv) + offsetof(rect, w),
            sizeof(gui_sprite_data),
            1));
        vbo->add_attribute_pointer(
            graphics::vertex_attribute_pointer(graphics::g_float,
                                               4,
                                               offsetof(gui_sprite_data, model),
                                               sizeof(gui_sprite_data),
                                               1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(gui_sprite_data, model) + sizeof(vec4),
            sizeof(gui_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(gui_sprite_data, model) + 2 * sizeof(vec4),
            sizeof(gui_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(gui_sprite_data, model) + 3 * sizeof(vec4),
            sizeof(gui_sprite_data),
            1));

        vao->add_vertex_buffer(vbo);

        auto sprite_element_buffer =
            std::make_shared<graphics::element_buffer>();
        sprite_element_buffer->bind();

        static std::vector<uint32_t> sprite_indexes{ 0, 2, 1, 0, 3, 2 };

        sprite_element_buffer->allocate_indexes(
            sprite_indexes.data(), 6, false);

        vao->set_elements(sprite_element_buffer);

        vbo->allocate_vertices(nullptr, vao_sprites_count, true);
        sc.registry.on_construct<gui_component>()
            .connect<&gui_system::sort_sprites>(*this);
        sc.registry.on_update<gui_component>()
            .connect<&gui_system::sort_sprites>(*this);
        sc.registry.on_construct<text_component>()
            .connect<&gui_system::sort_text>(*this);
        sc.registry.on_update<text_component>().connect<&gui_system::sort_text>(
            *this);
    }

    gui_system::~gui_system()
    {
        sc.registry.on_construct<gui_component>()
            .disconnect<&gui_system::sort_sprites>(*this);
        sc.registry.on_update<gui_component>()
            .disconnect<&gui_system::sort_sprites>(*this);
        sc.registry.on_construct<text_component>()
            .disconnect<&gui_system::sort_text>(*this);
        sc.registry.on_update<text_component>()
            .disconnect<&gui_system::sort_text>(*this);
    }

    void gui_system::draw_sprites(uint32_t                        count,
                                  res_ptr<gui_material_resource>& material,
                                  int                             layer)
    {
        if (count && material)
        {
            graphics::bind_material(
                static_resource_cast<material_resource>(material));
            if (vao_sprites_count < count)
            {
                vao_sprites_count = count;
                vbo->allocate_vertices(data.data(), count, true);
            }
            else
            {
                vbo->set_vertices(data.data(), 0, count);
            }
            vao->draw_instanced(graphics::draw_mode::triangles, count, 6);
        }
    }

    void gui_system::draw_text_sprites(uint32_t                count,
                                       res_ptr<font_resource>& font,
                                       int                     layer,
                                       vec4                    color)
    {
        if (count && font)
        {
            graphics::bind_font_texture(font, 0);
            graphics::bind_shader(graphics::text_shader);
            graphics::shader_type_any(color).uniform(
                graphics::text_shader->get_uniform_location(
                    graphics::text_color_property));
            if (vao_sprites_count < count)
            {
                vao_sprites_count = count;
                vbo->allocate_vertices(data.data(), count, true);
            }
            else
            {
                vbo->set_vertices(data.data(), 0, count);
            }
            vao->draw_instanced(graphics::draw_mode::triangles, count, 6);
        }
    }

    void gui_system::on_event(scene& sc, const event::game_event& g_event)
    {
        switch (g_event.g_type)
        {
            case event::type::mouse_button_event:
            {
                event::gui_event g_ev{};
                g_ev.mouse.button  = g_event.mouse.button;
                g_ev.mouse.clicks  = g_event.mouse.clicks;
                g_ev.mouse.pressed = g_event.mouse.pressed;
                auto gui_view      = sc.registry.view<gui_component>();
                for (auto [ent, ent_gui] : gui_view.each())
                {
                    if (ent_gui.is_hovered)
                    {
                        g_ev.gui_id = ent_gui.gui_event_id;
                        event::push_gui_event(g_ev);
                    }
                }
            }
            break;
            default:
                break;
        }
    }

    void gui_system::on_render(scene& sc, int delta_ms)
    {
        auto current_viewport = graphics::get_current_viewport();
        auto projection       = glm::ortho(0.f,
                                     static_cast<float>(current_viewport.x),
                                     0.f,
                                     static_cast<float>(current_viewport.y));
        graphics::update_gui_projection(projection);

        // gui render
        {
            auto gui_view =
                sc.registry.view<gui_component, rect_transform_component>();
            gui_view.use<gui_component>();

            vao->bind();
            vbo->bind();

            uint32_t                       size     = 0;
            res_ptr<gui_material_resource> material = nullptr;
            int                            layer    = 0;
            for (auto [ent, ent_gui, ent_transform] : gui_view.each())
            {
                if (ent_gui.get_material().get() != material.get() ||
                    ent_gui.layer != layer)
                {
                    draw_sprites(size, material, layer);
                    size     = 0;
                    material = ent_gui.get_material();
                    layer    = ent_gui.layer;
                }

                if (data.size() < size + 1)
                {
                    increase_data_size();
                }

                data[size].uv = ent_gui.uv;
                mat4x4 sprite_matrix(1);

                rect_transform world_transform;
                if (ent_transform.is_transform_valid())
                {
                    world_transform = ent_transform.get_last_transform();
                }
                else
                {
                    world_transform = ent_transform.get_world_transform();
                }

                sprite_matrix = glm::translate(sprite_matrix,
                                               { world_transform.start.x,
                                                 world_transform.start.y,
                                                 layer });

                sprite_matrix =
                    glm::scale(sprite_matrix, { world_transform.rect, 1.f });
                sprite_matrix = glm::translate(
                    sprite_matrix, { ent_gui.origin.x, ent_gui.origin.y, 0 });
                sprite_matrix =
                    glm::scale(sprite_matrix, { ent_gui.scale, 1.f });
                sprite_matrix =
                    glm::rotate(sprite_matrix, ent_gui.angle, { 0, 0, 1 });
                sprite_matrix = glm::translate(
                    sprite_matrix, { -ent_gui.origin.x, -ent_gui.origin.y, 0 });

                data[size].model = sprite_matrix;

                // gui hovered
                bool is_hovered =
                    input::mouse::x >= world_transform.start.x &&
                    input::mouse::x <=
                        world_transform.start.x + world_transform.rect.x &&
                    input::mouse::y >= world_transform.start.y &&
                    input::mouse::y <=
                        world_transform.start.y + world_transform.rect.y;

                if (ent_gui.is_hovered != is_hovered)
                {
                    event::gui_event g_ev{};
                    g_ev.gui_id        = ent_gui.gui_event_id;
                    g_ev.hover.hovered = is_hovered;
                    event::push_gui_event(g_ev);
                    ent_gui.is_hovered = is_hovered;
                }

                size++;
            }
            draw_sprites(size, material, layer);
        }

        // text render
        {
            auto text_view =
                sc.registry.view<text_component, rect_transform_component>();
            text_view.use<text_component>();

            uint32_t               size  = 0;
            res_ptr<font_resource> font  = nullptr;
            int                    layer = 0;
            vec4                   color = { 1, 1, 1, 1 };
            for (auto [ent, ent_text, ent_transform] : text_view.each())
            {
                if (ent_text.get_font().get() != font.get() ||
                    ent_text.layer != layer || ent_text.color != color)
                {

                    draw_text_sprites(size, font, layer, color);
                    size  = 0;
                    font  = ent_text.get_font();
                    layer = ent_text.layer;
                    color = ent_text.color;
                }

                rect_transform world_transform;
                if (ent_transform.is_transform_valid())
                {
                    world_transform = ent_transform.get_last_transform();
                }
                else
                {
                    world_transform = ent_transform.get_world_transform();
                }
                if (font)
                {
                    auto print_characters_count = unicode_strlen(ent_text.text);

                    while (data.size() < size + print_characters_count)
                    {
                        increase_data_size();
                    }

                    const float      rect_width   = world_transform.rect.x;
                    const float      rect_height  = world_transform.rect.y;
                    const float      line_advance = ent_text.line_advance;
                    const float      text_size    = ent_text.text_size;
                    const text_align align        = ent_text.align;

                    mat4x4 text_matrix(1);
                    text_matrix = glm::translate(
                        text_matrix, { world_transform.start, layer });
                    text_matrix =
                        glm::translate(text_matrix,
                                       { ent_text.origin.x * rect_width,
                                         ent_text.origin.y * rect_height,
                                         0 });
                    text_matrix =
                        glm::scale(text_matrix, { ent_text.scale, 1.f });
                    text_matrix =
                        glm::rotate(text_matrix, ent_text.angle, { 0, 0, 1 });
                    text_matrix =
                        glm::translate(text_matrix,
                                       { -ent_text.origin.x * rect_width,
                                         -ent_text.origin.y * rect_height,
                                         0 });

                    float text_scale =
                        glm::min(rect_width, rect_height) * text_size;

                    float text_y = rect_height - line_advance * text_scale;
                    float text_x = 0;

                    const char32_t* u_str = ent_text.text.data();
                    const char*     str   = (const char*)u_str;

                    const char*     line   = (const char*)ent_text.text.data();
                    const char32_t* u_line = (const char32_t*)line;

                    switch (align)
                    {
                        case text_align::left:
                            text_x = 0;
                            break;
                        case text_align::center:
                            text_x =
                                (rect_width -
                                 text_line_width(u_line, font) * text_scale) /
                                2;
                            break;
                        case text_align::right:
                            text_x = rect_width -
                                     text_line_width(u_line, font) * text_scale;
                            break;
                    }
                    int      i     = 0;
                    char32_t glyph = 0;
                    while ((glyph = next_glyph(u_str, &i)))
                    {
                        if (glyph == U'\n')
                        {
                            text_y -= line_advance * text_scale;
                            line   = str + i;
                            u_line = (const char32_t*)line;
                            switch (align)
                            {
                                case text_align::left:
                                    text_x = 0;
                                    break;
                                case text_align::center:
                                    text_x = (rect_width -
                                              text_line_width(u_line, font) *
                                                  text_scale) /
                                             2;
                                    break;
                                case text_align::right:
                                    text_x = rect_width -
                                             text_line_width(u_line, font) *
                                                 text_scale;
                                    break;
                            }
                        }
                        else
                        {
                            data[size].uv = font->get_glyph_uv(glyph);

                            rect  glyph_rect = font->get_glyph(glyph);
                            float advance    = font->get_glyph_advance(glyph);

                            mat4x4 sprite_matrix(1);
                            sprite_matrix = glm::translate(
                                sprite_matrix,
                                { text_x + glyph_rect.x * text_scale,
                                  text_y + glyph_rect.y * text_scale,
                                  layer });
                            sprite_matrix =
                                glm::scale(sprite_matrix,
                                           { glyph_rect.w * text_scale,
                                             glyph_rect.h * text_scale,
                                             1.f });
                            sprite_matrix    = text_matrix * sprite_matrix;
                            data[size].model = sprite_matrix;
                            text_x += advance * text_scale;
                            ++size;
                        }
                    }
                }

                // gui hovered
                bool is_hovered =
                    input::mouse::x >= world_transform.start.x &&
                    input::mouse::x <=
                        world_transform.start.x + world_transform.rect.x &&
                    input::mouse::y >= world_transform.start.y &&
                    input::mouse::y <=
                        world_transform.start.y + world_transform.rect.y;

                if (ent_text.is_hovered != is_hovered)
                {
                    event::gui_event g_ev{};
                    g_ev.gui_id        = ent_text.gui_event_id;
                    g_ev.hover.hovered = is_hovered;
                    event::push_gui_event(g_ev);
                    ent_text.is_hovered = is_hovered;
                }
            }
            draw_text_sprites(size, font, layer, color);
        }
    }

    std::size_t gui_system::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t gui_system::deserialize(std::istream& is)
    {
        return 0;
    }

    std::size_t gui_system::serialize_size() const
    {
        return 0;
    }

    void gui_system::increase_data_size()
    {
        max_sprites_count *= 2;
        data.resize(max_sprites_count);
    }

    void gui_system::sort_sprites(entt::registry& registry, entt::entity ent)
    {
        registry.sort<gui_component>(
            [](const gui_component& lhs, const gui_component& rhs)
            {
                if (lhs.layer == rhs.layer)
                    return lhs.get_material().get() < rhs.get_material().get();

                return lhs.layer < rhs.layer;
            });
    }

    void gui_system::sort_text(entt::registry& registry, entt::entity ent)
    {
        registry.sort<text_component>(
            [](const text_component& lhs, const text_component& rhs)
            {
                if (lhs.layer == rhs.layer)
                {
                    if (lhs.color == rhs.color)
                    {
                        return lhs.get_font().get() < rhs.get_font().get();
                    }
                    return lhs.color.r < rhs.color.r ||
                           lhs.color.g < rhs.color.g ||
                           lhs.color.b < rhs.color.b ||
                           lhs.color.a < rhs.color.a;
                }

                return lhs.layer < rhs.layer;
            });
    }
} // namespace Kengine