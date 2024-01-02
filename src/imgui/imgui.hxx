#pragma once

#ifdef KENGINE_IMGUI
namespace Kengine::imgui
{
    void registrate_imgui_render(on_imgui_render* func);

    void initialize();
    void draw();
    void shutdown();
} // namespace Kengine::imgui
#endif