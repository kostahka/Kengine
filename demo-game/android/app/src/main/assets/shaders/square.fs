#version 300 es
precision mediump float;

in vec4 vs_color;
in vec2 vs_text_coord;

uniform int       time;
uniform sampler2D checker;

out vec4 f_color;

void main()
{
    f_color = vec4(vs_color.r,
                   vs_color.g * (sin(float(time) / 500.0 + 1.0) / 2.0 + 0.5),
                   vs_color.ba);
    f_color = mix(f_color, texture(checker, vs_text_coord), f_color.g);
}