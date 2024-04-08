#include "Kengine/system/gui-system.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/components/gui-component.hxx"
#include "Kengine/components/rect-transform-component.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{

    gui_system::gui_system(scene& sc)
        : system(name, system_event_type | system_render_type)
        , sc(sc)
    {
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
    }

    gui_system::~gui_system()
    {
        sc.registry.on_construct<gui_component>()
            .disconnect<&gui_system::sort_sprites>(*this);
        sc.registry.on_update<gui_component>()
            .disconnect<&gui_system::sort_sprites>(*this);
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

    void gui_system::on_event(scene& sc, event::game_event g_event)
    {
        switch (g_event.g_type)
        {
            case event::type::window_resize:

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

            rect_transform world_transform =
                ent_transform.get_world_transform();

            sprite_matrix = glm::translate(
                sprite_matrix,
                { world_transform.start.x, world_transform.start.y, layer });

            sprite_matrix =
                glm::scale(sprite_matrix, { world_transform.rect, 1.f });
            sprite_matrix = glm::translate(
                sprite_matrix, { ent_gui.origin.x, ent_gui.origin.y, 0 });
            sprite_matrix = glm::scale(sprite_matrix, { ent_gui.scale, 1.f });
            sprite_matrix =
                glm::rotate(sprite_matrix, ent_gui.angle, { 0, 0, 1 });
            sprite_matrix = glm::translate(
                sprite_matrix, { -ent_gui.origin.x, -ent_gui.origin.y, 0 });

            data[size].model = sprite_matrix;

            size++;
        }
        draw_sprites(size, material, layer);
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

        auto gui_view =
            registry.view<gui_component, rect_transform_component>();
        gui_view.use<gui_component>();
    }
} // namespace Kengine