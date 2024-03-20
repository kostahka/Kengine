#include "Kengine/graphics/draw-primitives.hxx"

#include "../engine.hxx"
#include "../graphics/graphics.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/graphics/shader.hxx"
#include "Kengine/graphics/vertex-array.hxx"
#include "Kengine/graphics/vertex-buffer.hxx"
#include "Kengine/resources/shader-resource.hxx"

#include <memory>

namespace Kengine::graphics
{
    constexpr uint32_t max_vertices = 3 * 512;

    struct point_vertex
    {
        vec3  pos{};
        vec4  color{};
        float size = 1;
    };

    struct line_vertex
    {
        vec3 pos{};
        vec4 color{};
    };

    struct gl_render_points : public gl_render_primitive
    {
        gl_render_points(){};

        void create() override
        {
            program = std::make_shared<shader>(primitive_points_shader);

            vao = std::make_shared<vertex_array>();
            vao->bind();
            vbo = std::make_shared<vertex_buffer<point_vertex>>();
            vbo->bind();
            vbo->allocate_vertices(nullptr, max_vertices, true);
            vbo->add_attribute_pointer(vertex_attribute_pointer(
                g_float, 3, offsetof(point_vertex, pos), sizeof(point_vertex)));
            vbo->add_attribute_pointer(
                vertex_attribute_pointer(g_float,
                                         4,
                                         offsetof(point_vertex, color),
                                         sizeof(point_vertex)));
            vbo->add_attribute_pointer(
                vertex_attribute_pointer(g_float,
                                         1,
                                         offsetof(point_vertex, size),
                                         sizeof(point_vertex)));
            vao->add_vertex_buffer(vbo);
            m_count = 0;
        };

        void destroy() override
        {
            vao.reset();
            vbo.reset();
            program.reset();
        };

        void vertex(const vec& pos, const vec4& col, const float size) override
        {
            if (m_count == max_vertices)
                draw();

            vertices[m_count] = { pos, col, size };
            ++m_count;
        };

        void draw() override
        {
            if (m_count == 0)
                return;

            vbo->bind();
            vbo->set_vertices(vertices, 0, m_count);

            program->use();

            vao->bind();
            vao->draw(draw_mode::points, m_count);
            m_count = 0;
        };

        std::shared_ptr<shader>                      program;
        std::shared_ptr<vertex_array>                vao;
        std::shared_ptr<vertex_buffer<point_vertex>> vbo;
        point_vertex                                 vertices[max_vertices];
        uint32_t                                     m_count;
    };

    struct gl_render_lines : public gl_render_primitive
    {
        gl_render_lines(){};

        void create() override
        {
            program = std::make_shared<shader>(primitive_lines_shader);

            vao = std::make_shared<vertex_array>();
            vao->bind();
            vbo = std::make_shared<vertex_buffer<line_vertex>>();
            vbo->bind();
            vbo->allocate_vertices(nullptr, max_vertices, true);
            vbo->add_attribute_pointer(vertex_attribute_pointer(
                g_float, 3, offsetof(line_vertex, pos), sizeof(line_vertex)));
            vbo->add_attribute_pointer(vertex_attribute_pointer(
                g_float, 4, offsetof(line_vertex, color), sizeof(line_vertex)));
            vao->add_vertex_buffer(vbo);
            m_count = 0;
        };

        void destroy() override
        {
            vao.reset();
            vbo.reset();
            program.reset();
        };

        void vertex(const vec& pos, const vec4& col, const float size) override
        {
            if (m_count == max_vertices)
                draw();

            vertices[m_count] = { pos, col };
            ++m_count;
        };

        void draw() override
        {
            if (m_count == 0)
                return;

            vbo->bind();
            vbo->set_vertices(vertices, 0, m_count);

            program->use();
            vao->bind();
            vao->draw(draw_mode::lines, m_count);
            m_count = 0;
        };

        std::shared_ptr<shader>                     program;
        std::shared_ptr<vertex_array>               vao;
        std::shared_ptr<vertex_buffer<line_vertex>> vbo;
        line_vertex                                 vertices[max_vertices];
        uint32_t                                    m_count;
    };

    struct gl_render_triangles : public gl_render_primitive
    {
        gl_render_triangles(){};

        void create() override
        {
            program = std::make_shared<shader>(primitive_lines_shader);

            vao = std::make_shared<vertex_array>();
            vao->bind();
            vbo = std::make_shared<vertex_buffer<line_vertex>>();
            vbo->bind();
            vbo->allocate_vertices(nullptr, max_vertices, true);
            vbo->add_attribute_pointer(vertex_attribute_pointer(
                g_float, 3, offsetof(line_vertex, pos), sizeof(line_vertex)));
            vbo->add_attribute_pointer(vertex_attribute_pointer(
                g_float, 4, offsetof(line_vertex, color), sizeof(line_vertex)));
            vao->add_vertex_buffer(vbo);
            m_count = 0;
        };

        void destroy() override
        {
            vao.reset();
            vbo.reset();
            program.reset();
        };

        void vertex(const vec& pos, const vec4& col, const float size) override
        {
            if (m_count == max_vertices)
                draw();

            vertices[m_count] = { pos, col };
            ++m_count;
        };

        void draw() override
        {
            if (m_count == 0)
                return;

            vbo->bind();
            vbo->set_vertices(vertices, 0, m_count);

            program->use();
            vao->bind();
            vao->draw(draw_mode::triangles, static_cast<int>(m_count));
            m_count = 0;
        };

        std::shared_ptr<shader>                     program;
        std::shared_ptr<vertex_array>               vao;
        std::shared_ptr<vertex_buffer<line_vertex>> vbo;
        line_vertex                                 vertices[max_vertices];
        uint32_t                                    m_count;
    };

    gl_render_primitive* create_primitive_render(primitive_type type)
    {
        switch (type)
        {
            case primitive_type::points:
                return new gl_render_points();
                break;
            case primitive_type::lines:
                return new gl_render_lines();
                break;
            case primitive_type::triangles:
                return new gl_render_triangles();
                break;
            default:
                return nullptr;
                break;
        }
    };

    gl_render_primitive::~gl_render_primitive() = default;
} // namespace Kengine::graphics
