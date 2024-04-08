#pragma once

typedef void on_imgui_render();

#ifdef KENGINE_IMGUI
namespace Kengine::imgui
{
    E_DECLSPEC void set_cursor_visible(bool visible);
} // namespace Kengine::imgui
#endif