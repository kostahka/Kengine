#include "Kengine/graphics/texture-common.hxx"

namespace Kengine
{

    const char* get_texture_filter_str(texture_filter t_filter)
    {
#define ENUM_TO_STR_CASE(val)                                                  \
    case texture_filter::val:                                                  \
        return #val;

        switch (t_filter)
        {
            ENUM_TO_STR_CASE(nearest)
            ENUM_TO_STR_CASE(linear)
            ENUM_TO_STR_CASE(nearest_mipmap_nearest)
            ENUM_TO_STR_CASE(linear_mipmap_nearest)
            ENUM_TO_STR_CASE(nearest_mipmap_linear)
            ENUM_TO_STR_CASE(linear_mipmap_linear)
        }

        return "NONE";

#undef ENUM_TO_STR_CASE
    }

    const char* get_texture_internal_format_str(
        texture_internal_format t_internal_format)
    {
#define ENUM_TO_STR_CASE(val)                                                  \
    case texture_internal_format::val:                                         \
        return #val;

        switch (t_internal_format)
        {
            ENUM_TO_STR_CASE(r8)
            ENUM_TO_STR_CASE(r8ui)
            ENUM_TO_STR_CASE(r8i)
            ENUM_TO_STR_CASE(r16ui)
            ENUM_TO_STR_CASE(r16i)
            ENUM_TO_STR_CASE(r32ui)
            ENUM_TO_STR_CASE(r32i)

            ENUM_TO_STR_CASE(rg8)
            ENUM_TO_STR_CASE(rg8ui)
            ENUM_TO_STR_CASE(rg8i)
            ENUM_TO_STR_CASE(rg16ui)
            ENUM_TO_STR_CASE(rg16i)
            ENUM_TO_STR_CASE(rg32ui)
            ENUM_TO_STR_CASE(rg32i)

            ENUM_TO_STR_CASE(rgb8)
            ENUM_TO_STR_CASE(rgb565)

            ENUM_TO_STR_CASE(srgb8_a8)

            ENUM_TO_STR_CASE(rgba8)
            ENUM_TO_STR_CASE(rgb5_a1)
            ENUM_TO_STR_CASE(rgba4)
            ENUM_TO_STR_CASE(rgb10_a2)
            ENUM_TO_STR_CASE(rgba8ui)
            ENUM_TO_STR_CASE(rgba8i)
            ENUM_TO_STR_CASE(rgb10_a2ui)
            ENUM_TO_STR_CASE(rgba16ui)
            ENUM_TO_STR_CASE(rgba16i)
            ENUM_TO_STR_CASE(rgba32ui)
            ENUM_TO_STR_CASE(rgba32i)

            ENUM_TO_STR_CASE(depth16)
            ENUM_TO_STR_CASE(depth24)
            ENUM_TO_STR_CASE(depth32f)
            ENUM_TO_STR_CASE(depth24_stencil8)
            ENUM_TO_STR_CASE(depth32f_stencil8)
            ENUM_TO_STR_CASE(stencil8)
        }

        return "NONE";

#undef ENUM_TO_STR_CASE
    }

    const char* get_texture_format_str(texture_format t_format)
    {
#define ENUM_TO_STR_CASE(val)                                                  \
    case texture_format::val:                                                  \
        return #val;

        switch (t_format)
        {
            ENUM_TO_STR_CASE(none)
            ENUM_TO_STR_CASE(red)
            ENUM_TO_STR_CASE(rg)
            ENUM_TO_STR_CASE(rgb)
            ENUM_TO_STR_CASE(bgr)
            ENUM_TO_STR_CASE(rgba)
            ENUM_TO_STR_CASE(bgra)
            ENUM_TO_STR_CASE(depth)
            ENUM_TO_STR_CASE(depth_stencil)
        }

        return "NONE";

#undef ENUM_TO_STR_CASE
    }
} // namespace Kengine