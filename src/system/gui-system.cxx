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

        gui_data.resize(128);
        text_data.resize(128);

        sprite_vbo = std::make_shared<graphics::vertex_buffer<vec3>>();

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

        sprite_ebo = std::make_shared<graphics::element_buffer>();
        sprite_ebo->bind();

        create_gui_vao();
        create_text_vao();

        static std::vector<uint32_t> sprite_indexes{ 0, 2, 1, 0, 3, 2 };

        sprite_ebo->allocate_indexes(sprite_indexes.data(), 6, false);

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
            if (gui_vaos.size() <= gui_vao_number)
            {
                create_gui_vao();
            }
            auto& vao = gui_vaos[gui_vao_number];
            auto& vbo = gui_vbos[gui_vao_number];

            vbo->bind();
            vbo->allocate_vertices(gui_data.data(), count, true);

            graphics::render_packet packet(
                static_resource_cast<material_resource>(material));

            packet.vao = std::static_pointer_cast<graphics::vertex_array>(vao);
            packet.layer           = layer;
            packet.instances_count = count;
            packet.mode            = graphics::draw_mode::triangles;
            packet.vertices_count  = 6;
            packet.vertices_start  = 0;

            sc.get_main_pass().add_render_packet(packet);
            gui_vao_number++;
        }
    }

    void gui_system::draw_text_sprites(
        uint32_t count, res_ptr<font_material_resource>& material, int layer)
    {
        if (count && material)
        {
            if (text_vaos.size() <= text_vao_number)
            {
                create_text_vao();
            }
            auto& vao = text_vaos[text_vao_number];
            auto& vbo = text_vbos[text_vao_number];

            vbo->bind();
            vbo->allocate_vertices(text_data.data(), count, true);

            graphics::render_packet packet(
                static_resource_cast<material_resource>(material));

            packet.vao = std::static_pointer_cast<graphics::vertex_array>(vao);
            packet.layer           = layer;
            packet.instances_count = count;
            packet.mode            = graphics::draw_mode::triangles;
            packet.vertices_count  = 6;
            packet.vertices_start  = 0;

            sc.get_main_pass().add_render_packet(packet);
            text_vao_number++;
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
        gui_vao_number  = 0;
        text_vao_number = 0;

        auto  current_viewport = graphics::get_current_viewport();
        auto& cam              = sc.get_camera();
        auto  projection       = glm::ortho(0.f,
                                     static_cast<float>(current_viewport.x),
                                     0.f,
                                     static_cast<float>(current_viewport.y),
                                     cam.get_zNear(),
                                     cam.get_zFar());

        graphics::update_gui_projection(projection);

        // gui render
        {
            auto gui_view =
                sc.registry.view<gui_component, rect_transform_component>();
            gui_view.use<gui_component>();

            uint32_t                       size     = 0;
            res_ptr<gui_material_resource> material = nullptr;
            int                            layer    = 0;
            for (auto [ent, ent_gui, ent_transform] : gui_view.each())
            {
                rect_transform world_transform;
                world_transform = ent_transform.get_world_transform();

                if (world_transform.enabled)
                {

                    if (ent_gui.get_material().get() != material.get() ||
                        ent_gui.layer != layer)
                    {
                        draw_sprites(size, material, layer);
                        size     = 0;
                        material = ent_gui.get_material();
                        layer    = ent_gui.layer;
                    }

                    if (gui_data.size() < size + 1)
                    {
                        increase_gui_data_size();
                    }

                    gui_data[size].uv = ent_gui.uv;
                    mat4x4 sprite_matrix(1);

                    sprite_matrix = glm::translate(sprite_matrix,
                                                   { world_transform.start.x,
                                                     world_transform.start.y,
                                                     layer });

                    sprite_matrix = glm::scale(sprite_matrix,
                                               { world_transform.rect, 1.f });
                    sprite_matrix = glm::translate(
                        sprite_matrix,
                        { ent_gui.origin.x, ent_gui.origin.y, 0 });
                    sprite_matrix =
                        glm::rotate(sprite_matrix, ent_gui.angle, { 0, 0, 1 });
                    sprite_matrix =
                        glm::scale(sprite_matrix, { ent_gui.scale, 1.f });
                    sprite_matrix = glm::translate(
                        sprite_matrix,
                        { -ent_gui.origin.x, -ent_gui.origin.y, 0 });

                    gui_data[size].model = sprite_matrix;

                    // gui hovered
                    bool is_hovered =
                        input::mouse::x >= world_transform.start.x &&
                        input::mouse::x <=
                            world_transform.start.x + world_transform.rect.x &&
                        current_viewport.y - input::mouse::y >=
                            world_transform.start.y &&
                        current_viewport.y - input::mouse::y <=
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
            }
            draw_sprites(size, material, layer);
        }

        // text render
        {
            auto text_view =
                sc.registry.view<text_component, rect_transform_component>();
            text_view.use<text_component>();

            uint32_t                        size     = 0;
            res_ptr<font_material_resource> material = nullptr;
            int                             layer    = 0;
            for (auto [ent, ent_text, ent_transform] : text_view.each())
            {
                rect_transform world_transform;
                world_transform = ent_transform.get_world_transform();

                if (world_transform.enabled)
                {
                    if (ent_text.get_material().get() != material.get() ||
                        ent_text.layer != layer)
                    {

                        draw_text_sprites(size, material, layer);
                        size     = 0;
                        material = ent_text.get_material();
                        layer    = ent_text.layer;
                    }

                    if (material)
                    {
                        auto print_characters_count =
                            unicode_strlen(ent_text.text);

                        while (text_data.size() < size + print_characters_count)
                        {
                            increase_text_data_size();
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
                        text_matrix = glm::rotate(
                            text_matrix, ent_text.angle, { 0, 0, 1 });
                        text_matrix =
                            glm::scale(text_matrix, { ent_text.scale, 1.f });
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

                        const char* line = (const char*)ent_text.text.data();
                        const char32_t* u_line = (const char32_t*)line;

                        switch (align)
                        {
                            case text_align::left:
                                text_x = 0;
                                break;
                            case text_align::center:
                                text_x = (rect_width -
                                          text_line_width(
                                              u_line, material->get_font()) *
                                              text_scale) /
                                         2;
                                break;
                            case text_align::right:
                                text_x = rect_width -
                                         text_line_width(u_line,
                                                         material->get_font()) *
                                             text_scale;
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
                                        text_x =
                                            (rect_width -
                                             text_line_width(
                                                 u_line, material->get_font()) *
                                                 text_scale) /
                                            2;
                                        break;
                                    case text_align::right:
                                        text_x =
                                            rect_width -
                                            text_line_width(
                                                u_line, material->get_font()) *
                                                text_scale;
                                        break;
                                }
                            }
                            else
                            {
                                text_data[size].uv =
                                    material->get_font()->get_glyph_uv(glyph);

                                rect glyph_rect =
                                    material->get_font()->get_glyph(glyph);
                                float advance =
                                    material->get_font()->get_glyph_advance(
                                        glyph);

                                mat4x4 sprite_matrix(1);
                                sprite_matrix = glm::translate(
                                    sprite_matrix,
                                    { text_x + glyph_rect.x * text_scale,
                                      text_y + glyph_rect.y * text_scale,
                                      0 });
                                sprite_matrix =
                                    glm::scale(sprite_matrix,
                                               { glyph_rect.w * text_scale,
                                                 glyph_rect.h * text_scale,
                                                 1.f });
                                sprite_matrix = text_matrix * sprite_matrix;
                                text_data[size].model = sprite_matrix;
                                text_x += advance * text_scale;
                                text_data[size].color = ent_text.color;
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
            }
            draw_text_sprites(size, material, layer);
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

    void gui_system::increase_gui_data_size()
    {
        gui_data.resize(gui_data.size() * 2);
    }

    void gui_system::increase_text_data_size()
    {
        text_data.resize(text_data.size() * 2);
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
                    return lhs.get_material().get() < rhs.get_material().get();
                }

                return lhs.layer < rhs.layer;
            });
    }

    void gui_system::create_gui_vao()
    {
        auto vao = std::make_shared<graphics::vertex_element_array>();
        vao->bind();

        vao->add_vertex_buffer(sprite_vbo);

        auto vbo = std::make_shared<graphics::vertex_buffer<gui_sprite_data>>();

        vbo->bind();

        vbo->allocate_vertices(nullptr, 128, true);
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

        vao->set_elements(sprite_ebo);

        vao->unbind();

        gui_vaos.push_back(vao);
        gui_vbos.push_back(vbo);
    }

    void gui_system::create_text_vao()
    {
        auto vao = std::make_shared<graphics::vertex_element_array>();
        vao->bind();

        vao->add_vertex_buffer(sprite_vbo);

        auto vbo =
            std::make_shared<graphics::vertex_buffer<text_sprite_data>>();

        vbo->bind();

        vbo->allocate_vertices(nullptr, 128, true);
        vbo->add_attribute_pointer(
            graphics::vertex_attribute_pointer(graphics::g_float,
                                               2,
                                               offsetof(text_sprite_data, uv),
                                               sizeof(text_sprite_data),
                                               1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            2,
            offsetof(text_sprite_data, uv) + offsetof(rect, w),
            sizeof(text_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(text_sprite_data, color),
            sizeof(text_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(text_sprite_data, model),
            sizeof(text_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(text_sprite_data, model) + sizeof(vec4),
            sizeof(text_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(text_sprite_data, model) + 2 * sizeof(vec4),
            sizeof(text_sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(text_sprite_data, model) + 3 * sizeof(vec4),
            sizeof(text_sprite_data),
            1));

        vao->add_vertex_buffer(vbo);

        vao->set_elements(sprite_ebo);

        vao->unbind();

        text_vaos.push_back(vao);
        text_vbos.push_back(vbo);
    }

} // namespace Kengine