#pragma once

namespace Kengine
{
    enum class texture_filter
    {
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    enum class texture_internal_format
    {
        r8,
        r8ui,
        r8i,
        r16ui,
        r16i,
        r32ui,
        r32i,

        rg8,
        rg8ui,
        rg8i,
        rg16ui,
        rg16i,
        rg32ui,
        rg32i,

        rgb8,
        rgb565,

        srgb8_a8,

        rgba8,
        rgb5_a1,
        rgba4,
        rgb10_a2,
        rgba8ui,
        rgba8i,
        rgb10_a2ui,
        rgba16ui,
        rgba16i,
        rgba32ui,
        rgba32i,

        depth16,
        depth24,
        depth32f,
        depth24_stencil8,
        depth32f_stencil8,
        stencil8
    };

    enum class texture_format
    {
        none,
        red,
        rg,
        rgb,
        bgr,
        rgba,
        bgra,
        depth,
        depth_stencil,
    };

    E_DECLSPEC const char* get_texture_filter_str(texture_filter);
    E_DECLSPEC const char* get_texture_internal_format_str(
        texture_internal_format);
    E_DECLSPEC const char* get_texture_format_str(texture_format);
} // namespace Kengine