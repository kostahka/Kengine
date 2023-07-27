#pragma once

#include "Kengine/engine.hxx"
#include "Kengine/units/rect.hxx"
#include "shader-program.hxx"
#include "texture.hxx"
#include "vertex-array-object.hxx"

#include <vector>

namespace Kengine
{
    typedef void (*shader_program_use_func)(const glm::mat4& model);

    class sprite
    {
    public:
        sprite(texture_object*,
               const irect& uv,
               const vec2&  pos,
               const vec2&  size,
               bool         world_sprite = true,
               float        z            = 0);
        void draw() const;

        void set_origin(const vec2&);
        void set_uv(const irect&);
        void set_texture(texture_object*);
        void set_pos(const vec2&);
        void set_angle(const float);
        void set_size(const vec2&);

        [[nodiscard]] vec2            get_origin() const;
        [[nodiscard]] irect           get_uv() const;
        [[nodiscard]] texture_object* get_texture() const;
        [[nodiscard]] vec2            get_pos() const;
        [[nodiscard]] float           get_angle() const;
        [[nodiscard]] vec2            get_size() const;

        void set_program_use_func(shader_program_use_func use_func);

    private:
        void set_texture_coords();

        texture_object* texture;
        irect           uv;
        vec2            origin;
        vec2            pos;
        vec2            size;
        float           angle;
        float           z;

        shader_program_use_func use_func;
        vertex_array_object*    vao;
        vertex_text2d_array     vertices;
    };
} // namespace Kengine
