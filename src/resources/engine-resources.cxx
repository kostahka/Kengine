#include "engine-resources.hxx"

#include "../graphics/render-manager.hxx"

namespace Kengine::engine_resources
{
#ifdef ENGINE_DEV
    res_ptr<texture_resource>      game_texture_res;
    res_ptr<renderbuffer_resource> game_depth_res;
    res_ptr<framebuffer_resource>  game_frame_res;

    graphics::framebuffer game_frame;
#endif

    void initialize()
    {
#ifdef ENGINE_DEV
        game_texture_res = make_resource<texture_resource>(
            ivec2(800, 400), texture_format::rgba, "game_texture");

        game_depth_res = make_resource<renderbuffer_resource>(
            ivec2(800, 400),
            texture_internal_format::depth24_stencil8,
            "game_renderbuffer");

        game_frame_res = make_resource<framebuffer_resource>(
            static_resource_cast<resource>(game_texture_res),
            static_resource_cast<resource>(game_depth_res),
            "game_framebuffer");

        game_frame = graphics::framebuffer(
            game_frame_res, graphics::render_manager::clear_color);
#endif
    }

    void shutdown()
    {
#ifdef ENGINE_DEV
        game_frame       = graphics::framebuffer();
        game_texture_res = nullptr;
        game_depth_res   = nullptr;
        game_frame_res   = nullptr;
#endif
    }

} // namespace Kengine::engine_resources