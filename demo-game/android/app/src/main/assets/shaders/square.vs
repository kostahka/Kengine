#version 300 es
precision mediump float;

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
layout(location = 2) in vec2 v_text_coord;

layout (std140) uniform Matrices{
    mat4 projection;
    mat4 view;
};

out vec4 vs_color;
out vec2 vs_text_coord;

void main()
{
    gl_Position   = projection * view * vec4(v_pos, 1.0);
    vs_color      = v_color;
    vs_text_coord = v_text_coord;
}