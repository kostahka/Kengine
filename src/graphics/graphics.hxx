#pragma once

#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/sprite-component.hxx"
#include "Kengine/resources/shader-resource.hxx"
#include "Kengine/scene/scene.hxx"
#include "Kengine/units/matrix.hxx"
#include "Kengine/units/vector.hxx"

namespace Kengine::graphics
{
    extern res_ptr<vertex_shader_res>   sprite_vertex_shader;
    extern res_ptr<fragment_shader_res> sprite_fragment_shader;
    extern res_ptr<shader_res>          sprite_shader;

    extern res_ptr<vertex_shader_res>   gui_vertex_shader;
    extern res_ptr<fragment_shader_res> gui_fragment_shader;
    extern res_ptr<shader_res>          gui_shader;

    extern res_ptr<vertex_shader_res>   primitive_points_vertex_shader;
    extern res_ptr<fragment_shader_res> primitive_points_fragment_shader;
    extern res_ptr<shader_res>          primitive_points_shader;

    extern res_ptr<vertex_shader_res>   primitive_lines_vertex_shader;
    extern res_ptr<fragment_shader_res> primitive_lines_fragment_shader;
    extern res_ptr<shader_res>          primitive_lines_shader;

    extern vec4    clear_color;
    extern camera* default_camera;

    [[nodiscard]] bool initialize();
    void               shutdown();

    void  update_viewport();
    void  begin_render();
    void  end_render();
    void  update_matrices(const mat4x4& projection, const mat4x4& view);
    void  update_projection(const mat4x4& projection);
    void  update_gui_projection(const mat4x4& projection);
    void  update_view(const mat4x4& view);
    ivec2 get_current_viewport();
} // namespace Kengine::graphics