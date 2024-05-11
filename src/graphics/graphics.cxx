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
#include "Kengine/resources/font-resource.hxx"
#include "Kengine/resources/gui-material-resource.hxx"
#include "Kengine/resources/sprite-material-resource.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include <SDL3_ttf/SDL_ttf.h>

#include <list>
#include <memory>
#include <stack>

namespace Kengine::graphics
{
    res_ptr<vertex_shader_res>   sprite_vertex_shader   = nullptr;
    res_ptr<fragment_shader_res> sprite_fragment_shader = nullptr;
    res_ptr<shader_res>          sprite_shader          = nullptr;

    res_ptr<vertex_shader_res>   gui_vertex_shader   = nullptr;
    res_ptr<fragment_shader_res> gui_fragment_shader = nullptr;
    res_ptr<shader_res>          gui_shader          = nullptr;

    res_ptr<fragment_shader_res> text_fragment_shader = nullptr;
    res_ptr<shader_res>          text_shader          = nullptr;
    string_id                    text_color_property;

    res_ptr<vertex_shader_res>   primitive_lines_vertex_shader   = nullptr;
    res_ptr<fragment_shader_res> primitive_lines_fragment_shader = nullptr;
    res_ptr<shader_res>          primitive_lines_shader          = nullptr;

    res_ptr<vertex_shader_res>   primitive_points_vertex_shader   = nullptr;
    res_ptr<fragment_shader_res> primitive_points_fragment_shader = nullptr;
    res_ptr<shader_res>          primitive_points_shader          = nullptr;

    camera* default_camera = nullptr;

    vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

    static std::stack<framebuffer> framebuffers{};

    static std::unique_ptr<uniformbuffer_std140<mat4x4, mat4x4>>
        global_matrices = nullptr;
    static std::unique_ptr<uniformbuffer_std140<mat4x4>> global_gui_matrices =
        nullptr;

    bool initialize()
    {
        TTF_Init();

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

        global_gui_matrices =
            std::make_unique<uniformbuffer_std140<mat4x4>>(true, mat4x4(1));

        global_gui_matrices->bind_end_point(1);

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

        KENGINE_INFO("Create sprite_shader");

        sprite_shader->take_data();

        KENGINE_GL_CHECK(glUseProgram(sprite_shader->get_id()));
        sprite_shader->set_uniform_block_binding("Matrices", 0);

        gui_vertex_shader =
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
                };

                void main()
                {
                    out_tex_coord = in_position.xy * uv_size + uv_pos;
                    gl_Position = projection * model * vec4(in_position, 1.0);
                }
                )"),
                                             "gui_vertex");

        gui_fragment_shader =
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
                                               "gui_fragment");

        gui_shader = make_resource<shader_res>(
            gui_vertex_shader, gui_fragment_shader, "gui_shader");

        gui_shader->take_data();

        KENGINE_GL_CHECK(glUseProgram(gui_shader->get_id()));
        gui_shader->set_uniform_block_binding("Matrices", 1);

        text_fragment_shader =
            make_resource<fragment_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                in vec2 out_tex_coord;

                uniform vec4 color;
                uniform sampler2D texture0;

                out vec4 out_color;

                void main()
                {
                    out_color = texture(texture0, out_tex_coord) * color;
                }
            )"),
                                               "text_fragment");

        text_shader = make_resource<shader_res>(
            gui_vertex_shader, text_fragment_shader, "text_shader");

        text_shader->take_data();

        KENGINE_GL_CHECK(glUseProgram(text_shader->get_id()));
        text_shader->set_uniform_block_binding("Matrices", 1);

        text_color_property = hash_string("color");
        shader_type_any(vec4(1, 1, 1, 1))
            .uniform(text_shader->get_uniform_location(text_color_property));

        primitive_points_vertex_shader =
            make_resource<vertex_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                layout (location = 0) in vec3 v_position;
                layout (location = 1) in vec4 v_color;
                layout (location = 2) in float v_size;
                out vec4 f_color;

                layout (std140) uniform Matrices{
                    mat4 projection;
                    mat4 view;
                };

                void main()
                {
                    f_color = v_color;
                    gl_Position = projection * view * vec4(v_position, 1.0);
                    gl_PointSize = v_size;
                }
                )"),
                                             "primitive_points_vertex");

        primitive_points_fragment_shader =
            make_resource<fragment_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                in vec4 f_color;

                out vec4 fragColor;

                void main()
                {
                    fragColor = f_color;
                }
            )"),
                                               "primitive_points_fragment");

        primitive_points_shader =
            make_resource<shader_res>(primitive_points_vertex_shader,
                                      primitive_points_fragment_shader,
                                      "primitive_points_shader");

        primitive_points_shader->take_data();

        KENGINE_GL_CHECK(glUseProgram(primitive_points_shader->get_id()));
        primitive_points_shader->set_uniform_block_binding("Matrices", 0);

        primitive_lines_vertex_shader =
            make_resource<vertex_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                layout (location = 0) in vec3 v_position;
                layout (location = 1) in vec4 v_color;
                out vec4 f_color;

                layout (std140) uniform Matrices{
                    mat4 projection;
                    mat4 view;
                };

                void main()
                {
                    f_color = v_color;
                    gl_Position = projection * view * vec4(v_position, 1.0);
                }
                )"),
                                             "primitive_lines_vertex");

        primitive_lines_fragment_shader =
            make_resource<fragment_shader_res>(std::string_view(R"(
                #version 300 es
                precision mediump float;

                in vec4 f_color;

                out vec4 fragColor;

                void main()
                {
                    fragColor = f_color;
                }
            )"),
                                               "primitive_lines_fragment");

        primitive_lines_shader =
            make_resource<shader_res>(primitive_lines_vertex_shader,
                                      primitive_lines_fragment_shader,
                                      "primitive_lines_shader");

        primitive_lines_shader->take_data();

        KENGINE_GL_CHECK(glUseProgram(primitive_lines_shader->get_id()));
        primitive_lines_shader->set_uniform_block_binding("Matrices", 0);

        default_camera = new camera();

        return true;
    }

    void shutdown()
    {
        global_matrices.release();
        global_gui_matrices.release();
        TTF_Quit();
    }

    void begin_render()
    {
        set_clear_color(e_game->get_current_scene().clear_color);
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void bind_material(const res_ptr<gui_material_resource>& material)
    {
        const auto& textures   = material->get_textures();
        const auto& shader     = material->get_shader();
        const auto& properties = material->get_properties();

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

    void bind_material(const res_ptr<sprite_material_resource>& material)
    {
        const auto& textures   = material->get_textures();
        const auto& shader     = material->get_shader();
        const auto& properties = material->get_properties();

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

    void bind_material(const res_ptr<material_resource>& material)
    {
        const auto& textures   = material->get_textures();
        const auto& shader     = material->get_shader();
        const auto& properties = material->get_properties();

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

    void bind_shader(const res_ptr<shader_res>& shader)
    {
        KENGINE_GL_CHECK(glUseProgram(shader->get_id()));
    };

    void bind_font_texture(const res_ptr<font_resource>& font,
                           uint32_t                      block_index)
    {
        KENGINE_GL_CHECK(glActiveTexture(GL_TEXTURE0 + block_index));
        KENGINE_GL_CHECK(glBindTexture(GL_TEXTURE_2D, font->get_id()));
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
        global_matrices->bind();
        global_matrices->set_values(projection, view);
    }

    void update_projection(const mat4x4& projection)
    {
        global_matrices->bind();
        global_matrices->set_value<0>(projection);
    }

    void update_gui_projection(const mat4x4& projection)
    {
        global_gui_matrices->bind();
        global_gui_matrices->set_value<0>(projection);
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

    ivec2 get_current_viewport()
    {
        if (framebuffers.size() > 0)
        {
            return framebuffers.top().get_size();
        }
        else
        {
            return window::get_size();
        }
    }
} // namespace Kengine::graphics