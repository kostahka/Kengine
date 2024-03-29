#include "Kengine/render/sprite.hxx"

#include "../engine.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/render/engine-resources.hxx"
#include "Kengine/render/vertex-array-object.hxx"

#include "glm/ext/matrix_transform.hpp"

namespace Kengine
{

    constexpr vec v1{ 0, 0, 0 };
    constexpr vec v2{ 0, 1, 0 };
    constexpr vec v3{ 1, 1, 0 };
    constexpr vec v4{ 1, 0, 0 };

    void world_sprite_program_use(const glm::mat4& model)
    {
        e_resources::global_sprite_program->use();
        // e_resources::global_sprite_program->set_uniform_matrix4fv(
        //     "projection", Kengine::e_game->projection);
        // e_resources::global_sprite_program->set_uniform_matrix4fv(
        //     "view", Kengine::e_game->view);
        e_resources::global_sprite_program->set_uniform_matrix4fv("model",
                                                                  model);
    }

    void local_sprite_program_use(const glm::mat4& model)
    {
        e_resources::local_sprite_program->use();
        // e_resources::local_sprite_program->set_uniform_matrix4fv(
        //     "projection", Kengine::e_game->projection);
        e_resources::local_sprite_program->set_uniform_matrix4fv("model",
                                                                 model);
    }

    sprite::sprite(texture_object* texture,
                   const irect&    uv,
                   const vec2&     pos,
                   const vec2&     size,
                   bool            world_sprite,
                   float           z)
        : texture(texture)
        , uv(uv)
        , pos(pos)
        , size(size)
        , origin({ 0, 0 })
        , angle(0)
        , z(z)
    {
        vertices.resize(6);

        ivec2 tex_size = texture->get_size();

        const vec2 t1 = { static_cast<float>(uv.x) / tex_size.x,
                          static_cast<float>(uv.y + uv.h) / tex_size.y };
        const vec2 t2 = { static_cast<float>(uv.x) / tex_size.x,
                          static_cast<float>(uv.y) / tex_size.y };
        const vec2 t3 = { static_cast<float>(uv.x + uv.w) / tex_size.x,
                          static_cast<float>(uv.y) / tex_size.y };
        const vec2 t4 = { static_cast<float>(uv.x + uv.w) / tex_size.x,
                          static_cast<float>(uv.y + uv.h) / tex_size.y };

        vertices[0] = { v1, t1 };
        vertices[1] = { v2, t2 };
        vertices[2] = { v3, t3 };

        vertices[3] = { v3, t3 };
        vertices[4] = { v4, t4 };
        vertices[5] = { v1, t1 };

        vao = create_vao(vertices);
        if (world_sprite)
        {
            use_func = world_sprite_program_use;
        }
        else
        {
            use_func = local_sprite_program_use;
        }
    };

    void sprite::draw() const
    {
        texture->bind();
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(pos.x, pos.y, z));
        model = glm::rotate(model, angle, glm::vec3(0.0, 0.0, 1.0));
        model = glm::scale(model, glm::vec3(size.x, size.y, 1));
        model = glm::translate(model, glm::vec3(-origin.x, -origin.y, 0));
        use_func(model);
        vao->draw_triangles(2);
    };

    void sprite::set_origin(const vec2& origin)
    {
        this->origin = origin;
    };

    void sprite::set_uv(const irect& uv)
    {
        this->uv = uv;
        set_texture_coords();
    };

    void sprite::set_texture(texture_object* texture)
    {
        this->texture = texture;
        set_texture_coords();
    };

    void sprite::set_pos(const vec2& pos)
    {
        this->pos = pos;
    };

    void sprite::set_angle(const float angle)
    {
        this->angle = angle;
    };

    void sprite::set_size(const vec2& size)
    {
        this->size = size;
    };

    vec2 sprite::get_origin() const
    {
        return origin;
    };

    irect sprite::get_uv() const
    {
        return uv;
    };

    texture_object* sprite::get_texture() const
    {
        return texture;
    };

    vec2 sprite::get_pos() const
    {
        return pos;
    };

    float sprite::sprite::get_angle() const
    {
        return angle;
    };

    vec2 sprite::get_size() const
    {
        return size;
    }

    void sprite::set_texture_coords()
    {
        ivec2 tex_size = texture->get_size();

        const vec2 t1 = { static_cast<float>(uv.x) / tex_size.x,
                          static_cast<float>(uv.y + uv.h) / tex_size.y };
        const vec2 t2 = { static_cast<float>(uv.x) / tex_size.x,
                          static_cast<float>(uv.y) / tex_size.y };
        const vec2 t3 = { static_cast<float>(uv.x + uv.w) / tex_size.x,
                          static_cast<float>(uv.y) / tex_size.y };
        const vec2 t4 = { static_cast<float>(uv.x + uv.w) / tex_size.x,
                          static_cast<float>(uv.y + uv.h) / tex_size.y };

        vertices[0] = { v1, t1 };
        vertices[1] = { v2, t2 };
        vertices[2] = { v3, t3 };

        vertices[3] = { v3, t3 };
        vertices[4] = { v4, t4 };
        vertices[5] = { v1, t1 };

        vao->set_vertices(
            vertices.data(), 0, vertices.size() * sizeof(vertex_text2d));
    }

    void sprite::set_program_use_func(shader_program_use_func use_func)
    {
        this->use_func = use_func;
    }
} // namespace Kengine
