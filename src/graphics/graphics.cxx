#include "Kengine/graphics/graphics.hxx"
#include "graphics.hxx"

#include "../opengl/opengl-debug.hxx"
#include "../window/window.hxx"
#include "Kengine/components/render-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/graphics/uniformbuffer.hxx"
#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include <list>
#include <memory>
#include <stack>

namespace Kengine::graphics
{
    res_ptr<vertex_shader_res>   sprite_vertex_shader   = nullptr;
    res_ptr<fragment_shader_res> sprite_fragment_shader = nullptr;
    res_ptr<shader_res>          sprite_shader          = nullptr;

    vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

    struct sprite_data
    {
        rect   uv;
        mat4x4 model;
    };

    static std::unordered_map<string_id, std::list<entt::entity>> sprites_map{};
    static std::unordered_map<string_id, uint32_t>     sprite_count_map{};
    static std::unique_ptr<vertex_element_array>       sprite_vao = nullptr;
    static std::vector<sprite_data>                    sprite_component_data{};
    static std::shared_ptr<vertex_buffer<sprite_data>> sprite_components_vbo =
        nullptr;
    static uint32_t sprite_max_count = 100;

    static std::stack<framebuffer> framebuffers{};

    static std::unique_ptr<uniformbuffer_std140<mat4x4, mat4x4>>
        global_matrices = nullptr;

    void add_sprite(sprite_component* sprite)
    {
        const auto material_id = sprite->get_material()->get_resource_id();
        const auto ent         = sprite->get_entity();

        sprites_map[material_id].push_back(ent);
        sprite_count_map[material_id]++;
    }

    void remove_sprite(sprite_component* sprite)
    {
        const auto material_id = sprite->get_material()->get_resource_id();
        const auto ent         = sprite->get_entity();

        sprites_map[material_id].remove(ent);
        sprite_count_map[material_id]--;
    }

    bool initialize()
    {
        KENGINE_GL_CHECK(glEnable(GL_DEPTH_TEST));
        KENGINE_GL_CHECK(glDepthFunc(GL_ALWAYS));
        KENGINE_GL_CHECK(glEnable(GL_BLEND));
        KENGINE_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        KENGINE_GL_CHECK(glClearColor(
            clear_color.r, clear_color.g, clear_color.b, clear_color.a));

        global_matrices =
            std::make_unique<uniformbuffer_std140<mat4x4, mat4x4>>(
                true, mat4x4(1), mat4x4(1));

        global_matrices->bind_end_point(0);

        sprite_vertex_shader =
            make_resource<vertex_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                layout (location = 0) in vec3 in_position;
                layout (location = 1) in vec2 uv_pos;
                layout (location = 2) in vec2 uv_size;
                layout (location = 3) in mat4 model;
               
                out vec2 out_tex_coord;

                layout (std140) uniform Matrices{
                    mat4 projection;
                    mat4 view;
                };

                void main()
                {
                    out_tex_coord = in_position.xy * uv_size + uv_pos;
                    gl_Position = projection * view * model * vec4(in_position, 1.0);
                }
                )"),
                                             "sprite_vertex");

        sprite_fragment_shader =
            make_resource<fragment_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                in vec2 out_tex_coord;

                uniform sampler2D texture0;

                out vec4 out_color;

                void main()
                {
                    out_color = texture(texture0, out_tex_coord);
                }
            )"),
                                               "sprite_fragment");

        sprite_shader = make_resource<shader_res>(
            sprite_vertex_shader, sprite_fragment_shader, "sprite_shader");

        sprite_shader->set_uniform_block_binding("Matrices", 0);

        KENGINE_GL_CHECK(glUseProgram(sprite_shader->get_id()));

        sprite_vao = std::make_unique<vertex_element_array>();
        sprite_vao->bind();

        auto sprite_vbo = std::make_shared<vertex_buffer<vec3>>();

        static std::vector<vec3> sprite_pos_vertices{
            {0,  0, 0},
            { 0, 1, 0},
            { 1, 1, 0},
            { 1, 0, 0},
        };

        sprite_vbo->bind();
        sprite_vbo->allocate_vertices(sprite_pos_vertices.data(), 4, false);
        sprite_vbo->add_attribute_pointer(
            vertex_attribute_pointer(g_float, 3, 0, sizeof(vec3)));

        sprite_vao->add_vertex_buffer(sprite_vbo);

        sprite_components_vbo = std::make_shared<vertex_buffer<sprite_data>>();

        sprite_component_data.resize(sprite_max_count);

        sprite_components_vbo->bind();

        sprite_components_vbo->allocate_vertices(
            sprite_component_data.data(), sprite_max_count, true);
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float, 2, offsetof(sprite_data, uv), sizeof(sprite_data), 1));
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float,
            2,
            offsetof(sprite_data, uv) + offsetof(rect, w),
            sizeof(sprite_data),
            1));
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float, 4, offsetof(sprite_data, model), sizeof(sprite_data), 1));
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float,
            4,
            offsetof(sprite_data, model) + sizeof(vec4),
            sizeof(sprite_data),
            1));
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float,
            4,
            offsetof(sprite_data, model) + 2 * sizeof(vec4),
            sizeof(sprite_data),
            1));
        sprite_components_vbo->add_attribute_pointer(vertex_attribute_pointer(
            g_float,
            4,
            offsetof(sprite_data, model) + 3 * sizeof(vec4),
            sizeof(sprite_data),
            1));

        sprite_vao->add_vertex_buffer(sprite_components_vbo);

        auto sprite_element_buffer = std::make_shared<element_buffer>();
        sprite_element_buffer->bind();

        static std::vector<uint32_t> sprite_indexes{ 0, 2, 1, 0, 3, 2 };

        sprite_element_buffer->allocate_indexes(
            sprite_indexes.data(), 6, false);

        sprite_vao->set_elements(sprite_element_buffer);

        return true;
    }

    void shutdown()
    {
        global_matrices.release();
        sprite_vao.release();
        sprite_components_vbo.reset();
    }

    void begin_render()
    {
        // TODO
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void bind_material(res_ptr<material_resource>& material)
    {
        const auto textures   = material->get_textures();
        const auto shader     = material->get_shader();
        const auto properties = material->get_properties();

        for (auto& [block, texture] : textures)
        {
            KENGINE_GL_CHECK(glActiveTexture(GL_TEXTURE0 + block));
            KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture->get_id()));
        }

        KENGINE_GL_CHECK(glUseProgram(shader->get_id()));

        const auto uniform_locations = shader->get_uniform_locations();
        for (auto& [name_id, property] : properties)
        {
            auto uniform_location = uniform_locations.find(name_id);
            if (uniform_location != uniform_locations.end())
            {
                property.uniform(uniform_location->second);
            }
        }
    };

    void on_render(int delta_ms)
    {
        auto current_scene = scene_manager::get_current_scene();

        auto render_view = current_scene->registry.view<render_component>();
        for (auto [entity, ent_render] : render_view.each())
        {
            bind_material(ent_render.material);

            ent_render.vao->bind();
            ent_render.vao->draw(ent_render.draw_mode,
                                 ent_render.vertices_count,
                                 ent_render.vertices_start);
        };

        auto sprite_view = current_scene->registry
                               .view<transform_component, sprite_component>();

        sprite_vao->bind();
        sprite_components_vbo->bind();

        for (auto& [material_id, entities] : sprites_map)
        {
            const uint32_t sprites_count = sprite_count_map[material_id];

            if (sprites_count > sprite_max_count)
            {
                sprite_max_count *= 2;
                sprite_component_data.resize(sprite_max_count);
                sprite_components_vbo->allocate_vertices(
                    sprite_component_data.data(), sprite_max_count, true);
            }

            if (sprites_count > 0)
            {
                res_ptr<material_resource> material =
                    static_resource_cast<material_resource>(
                        resource_manager::get_resource(material_id));

                bind_material(material);

                uint32_t sprite_index = 0;

                for (auto& entity : entities)
                {
                    auto ent_iterator = sprite_view.find(entity);
                    if (ent_iterator != sprite_view.end())
                    {
                        auto [ent_transform, ent_sprite] =
                            sprite_view.get(entity);

                        mat4x4 sprite_matrix(1);
                        sprite_matrix = glm::translate(
                            sprite_matrix,
                            { -ent_sprite.origin.x, -ent_sprite.origin.y, 0 });
                        sprite_matrix =
                            glm::scale(sprite_matrix, ent_transform.scale);
                        sprite_matrix = glm::rotate(
                            sprite_matrix, ent_transform.angle, { 0, 0, 1 });
                        sprite_matrix = glm::translate(sprite_matrix,
                                                       ent_transform.position);

                        sprite_component_data[sprite_index].model =
                            sprite_matrix;
                        sprite_component_data[sprite_index].uv = ent_sprite.uv;
                        sprite_index++;
                    }
                }

                sprite_components_vbo->set_vertices(
                    sprite_component_data.data(), 0, sprite_index);
                sprite_vao->draw_instanced(
                    draw_mode::triangles, sprite_index, 6);
            }
        };
    }

    void end_render()
    {
        // TODO
        SDL_GL_SwapWindow(window::window);
    }

    void set_clear_color(const vec4& col)
    {
        if (framebuffers.size() > 0)
        {
            framebuffers.top().set_clear_color(col);
        }
        else
        {
            clear_color = col;
        }
        KENGINE_GL_CHECK(glClearColor(col.r, col.g, col.b, col.a));
    }

    void push_framebuffer(framebuffer& frame)
    {
        framebuffers.push(frame);
        KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame.get_id()));
        frame.clear();
        auto frame_size = frame.get_size();
        KENGINE_GL_CHECK(glViewport(0, 0, frame_size.x, frame_size.y));
    }

    void pop_framebuffer()
    {
        framebuffers.pop();
        if (framebuffers.size() > 0)
        {
            KENGINE_GL_CHECK(
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.top().get_id()));
        }
        else
        {
            KENGINE_GL_CHECK(glClearColor(
                clear_color.r, clear_color.g, clear_color.b, clear_color.a));
            KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        update_viewport();
    }

    void update_matrices(const mat4x4& projection, const mat4x4& view)
    {
        global_matrices->set_values(projection, view);
    }

    void update_projection(const mat4x4& projection)
    {
        global_matrices->set_value<0>(projection);
    }

    void update_view(const mat4x4& view)
    {
        global_matrices->set_value<1>(view);
    }

    void update_viewport()
    {
        if (framebuffers.size() > 0)
        {
            auto frame_size = framebuffers.top().get_size();
            KENGINE_GL_CHECK(glViewport(0, 0, frame_size.x, frame_size.y));
        }
        else
        {
            auto wnd_size = window::get_size();
            KENGINE_GL_CHECK(glViewport(0, 0, wnd_size.x, wnd_size.y));
        }
    }
} // namespace Kengine::graphics