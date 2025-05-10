#pragma once

#include "imgui_internal.h"
#include <cstdint>
#include <imgui.h>

enum class game_play_mode
{
    play,
    paused,
    stopped
};

class toolbar_wnd
{
public:
    static constexpr const char* window_name = "Toolbar";

    toolbar_wnd();

    void display(uint32_t& b2_debug_draw_flags);

    inline bool is_render_properties_changed() const
    {
        return render_properties_changed;
    }

    bool gui_debug_draw = false;
    bool physics_debug_draw = false;

private:
    bool           render_properties_changed = false;
    ImGuiAxis      axis                      = ImGuiAxis_X;
    game_play_mode play_mode                 = game_play_mode::stopped;
};