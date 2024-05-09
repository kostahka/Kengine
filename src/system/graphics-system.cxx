#include "Kengine/system/graphics-system.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/components/animation-component.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{

    graphics_system::graphics_system(scene& sc)
        : system(name)
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

        vbo = std::make_shared<graphics::vertex_buffer<sprite_data>>();

        data.resize(max_sprites_count);

        vbo->bind();

        vbo->allocate_vertices(data.data(), max_sprites_count, true);
        vbo->add_attribute_pointer(
            graphics::vertex_attribute_pointer(graphics::g_float,
                                               2,
                                               offsetof(sprite_data, uv),
                                               sizeof(sprite_data),
                                               1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            2,
            offsetof(sprite_data, uv) + offsetof(rect, w),
            sizeof(sprite_data),
            1));
        vbo->add_attribute_pointer(
            graphics::vertex_attribute_pointer(graphics::g_float,
                                               4,
                                               offsetof(sprite_data, model),
                                               sizeof(sprite_data),
                                               1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(sprite_data, model) + sizeof(vec4),
            sizeof(sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(sprite_data, model) + 2 * sizeof(vec4),
            sizeof(sprite_data),
            1));
        vbo->add_attribute_pointer(graphics::vertex_attribute_pointer(
            graphics::g_float,
            4,
            offsetof(sprite_data, model) + 3 * sizeof(vec4),
            sizeof(sprite_data),
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

        sc.registry.on_construct<sprite_component>()
            .connect<&graphics_system::sort_sprites>(*this);
        sc.registry.on_update<sprite_component>()
            .connect<&graphics_system::sort_sprites>(*this);
    }

    graphics_system::~graphics_system()
    {
        sc.registry.on_construct<sprite_component>()
            .disconnect<&graphics_system::sort_sprites>(*this);
        sc.registry.on_update<sprite_component>()
            .disconnect<&graphics_system::sort_sprites>(*this);
    }

    void graphics_system::draw_sprites(
        uint32_t count, res_ptr<sprite_material_resource>& material, int layer)
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

    void graphics_system::on_start(scene& sc) {}

    void graphics_system::on_event(scene& sc, const event::game_event& g_event)
    {
        switch (g_event.g_type)
        {
            case event::type::window_resize:
            {
                update_projections = true;
            }
            break;

            default:
                break;
        }
    }

    void graphics_system::on_render(scene& sc, int delta_ms)
    {
        auto anim_view =
            sc.registry.view<animation_component, sprite_component>();
        for (auto [ent, ent_anim, ent_sprite] : anim_view.each())
        {
            if (ent_anim.is_playing)
            {
                ent_anim.animation_time += delta_ms;
                int next_frame =
                    ent_anim.get_current_frame() +
                    ent_anim.animation_time / ent_anim.delta_frame_time;
                ent_anim.set_current_frame(next_frame);
                if (!ent_anim.is_looped &&
                    next_frame >= ent_anim.get_anim_res()
                                      ->get_animations()
                                      .find(ent_anim.get_current_animation())
                                      ->second.size())
                {
                    ent_anim.is_playing = false;
                }
                ent_anim.animation_time =
                    ent_anim.animation_time % ent_anim.delta_frame_time;
                ent_sprite.uv = ent_anim.get_current_uv();
            }
        }

        if (update_projections)
        {
            auto camera_view = sc.registry.view<camera_component>();
            for (auto [ent, cam] : camera_view.each())
            {
                cam.camera.calculate_projection();
            }
            update_projections = false;
        }

        if (!sc.get_camera().is_projection_valid())
        {
            sc.get_camera().calculate_projection();
        }

        graphics::update_matrices(sc.get_camera().get_projection(),
                                  sc.get_camera().view);

        auto render_view = sc.registry.view<render_component>();
        for (auto [entity, ent_render] : render_view.each())
        {
            graphics::bind_material(ent_render.material);

            ent_render.vao->bind();
            ent_render.vao->draw(ent_render.draw_mode,
                                 ent_render.vertices_count,
                                 ent_render.vertices_start);
        };

        auto sprite_view =
            sc.registry.view<sprite_component, transform_component>();
        sprite_view.use<sprite_component>();

        vao->bind();
        vbo->bind();

        uint32_t                          size     = 0;
        res_ptr<sprite_material_resource> material = nullptr;
        int                               layer    = 0;
        for (auto [ent, sprite, ent_transform] : sprite_view.each())
        {
            if (sprite.visible)
            {
                if (sprite.get_material().get() != material.get() ||
                    sprite.layer != layer)
                {
                    draw_sprites(size, material, layer);
                    size     = 0;
                    material = sprite.get_material();
                    layer    = sprite.layer;
                }

                if (data.size() < size + 1)
                {
                    increase_data_size();
                }

                data[size].uv = sprite.uv;
                mat4x4 sprite_matrix(1);

                transform world_transform = ent_transform.get_world_transform();

                sprite_matrix = glm::translate(sprite_matrix,
                                               { world_transform.position.x,
                                                 world_transform.position.y,
                                                 layer });
                sprite_matrix =
                    glm::scale(sprite_matrix, world_transform.scale);
                sprite_matrix = glm::rotate(
                    sprite_matrix, world_transform.angle, { 0, 0, 1 });
                sprite_matrix = glm::translate(
                    sprite_matrix, { -sprite.origin.x, -sprite.origin.y, 0 });

                data[size].model = sprite_matrix;

                size++;
            }
        }
        draw_sprites(size, material, layer);
    }

    void graphics_system::on_update(scene& sc, int delta_ms)
    {
        auto camera_view =
            sc.registry.view<transform_component, camera_component>();
        for (auto [entity, ent_transform, ent_camera] : camera_view.each())
        {
            transform  cam_transform = ent_transform.get_world_transform();
            const vec3 eye           = { cam_transform.position.x,
                                         cam_transform.position.y,
                                         0 };
            const vec3 center        = { eye.x, eye.y, eye.z - 1 };
            const vec3 up            = { std::sin(-cam_transform.angle),
                                         std::cos(-cam_transform.angle),
                                         0 };

            const auto view_matrix = glm::lookAt(eye, center, up);

            ent_camera.camera.view = view_matrix;
        };
    }

    std::size_t graphics_system::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t graphics_system::deserialize(std::istream& is)
    {
        return 0;
    }

    std::size_t graphics_system::serialize_size() const
    {
        return 0;
    }

    void graphics_system::increase_data_size()
    {
        max_sprites_count *= 2;
        data.resize(max_sprites_count);
    }

    void graphics_system::sort_sprites(entt::registry& registry,
                                       entt::entity    ent)
    {
        registry.sort<sprite_component>(
            [](const sprite_component& lhs, const sprite_component& rhs)
            {
                if (lhs.layer == rhs.layer)
                    return lhs.get_material().get() < rhs.get_material().get();

                return lhs.layer < rhs.layer;
            });

        auto sprite_view =
            registry.view<sprite_component, transform_component>();
        sprite_view.use<sprite_component>();
    }
} // namespace Kengine