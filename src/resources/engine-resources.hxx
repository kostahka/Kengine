#pragma once

#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/resources/framebuffer-resource.hxx"
#include "Kengine/resources/res-ptr.hxx"

namespace Kengine::engine_resources
{
#ifdef ENGINE_DEV
    extern res_ptr<texture_resource>      game_texture_res;
    extern res_ptr<renderbuffer_resource> game_depth_res;
    extern res_ptr<framebuffer_resource>  game_frame_res;

    extern graphics::framebuffer game_frame;
#endif
    void initialize();
    void shutdown();
} // namespace Kengine::engine_resources