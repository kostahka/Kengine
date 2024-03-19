#include "Kengine/graphics/graphics.hxx"
#include "graphics.hxx"

#include "../engine.hxx"
#include "../opengl/opengl-debug.hxx"
#include "../window/window.hxx"
#include "Kengine/components/camera-component.hxx"
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

    camera_component* default_camera_component = nullptr;

    vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

    static std::stack<framebuffer> framebuffers{};

    static std::unique_ptr<uniformbuffer_std140<mat4x4, mat4x4>>
        global_matrices = nullptr;

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

        KENGINE_GL_CHECK(glUseProgram(sprite_shader->get_id()));
        sprite_shader->set_uniform_block_binding("Matrices", 0);

        default_camera_component = new camera_component();

        return true;
    }

    void shutdown()
    {
        global_matrices.release();
    }

    void begin_render()
    {
        set_clear_color(e_game->get_current_scene().clear_color);
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void bind_material(const res_ptr<material_resource>& material)
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

    void on_update(scene& sc, int delta_ms)
    {
        auto camera_view =
            sc.registry.view<transform_component, camera_component>();
        for (auto [entity, ent_transform, ent_camera] : camera_view.each())
        {
            const vec3 eye    = { ent_transform.position.x,
                                  ent_transform.position.y,
                                  -1 };
            const vec3 center = { eye.x, eye.y, eye.z - 1 };
            const vec3 up     = { std::sin(ent_transform.angle),
                                  std::cos(ent_transform.angle),
                                  0 };

            const auto view_matrix = glm::lookAt(eye, center, up);

            ent_camera.camera.view = view_matrix;
        };
    }

    void on_render(scene& sc, int delta_ms)
    {
        auto render_view = sc.registry.view<render_component>();
        for (auto [entity, ent_render] : render_view.each())
        {
            bind_material(ent_render.material);

            ent_render.vao->bind();
            ent_render.vao->draw(ent_render.draw_mode,
                                 ent_render.vertices_count,
                                 ent_render.vertices_start);
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