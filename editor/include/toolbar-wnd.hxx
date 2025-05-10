#pragma once

#include "imgui_internal.h"
#include <cstdint>
#include <imgui.h>
#include <memory>

enum class toolbar_game_state
{
    play,
    paused,
    stopped
};

class toolbar_game_state_listener
{
public:
    virtual void game_state_changed(toolbar_game_state old_state,
                                    toolbar_game_state new_state) = 0;
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

    inline void set_game_state_listener(
        std::shared_ptr<toolbar_game_state_listener> listener)
    {
        this->listener = listener;
    }

    bool gui_debug_draw     = false;
    bool physics_debug_draw = false;

private:
    std::shared_ptr<toolbar_game_state_listener> listener{};
    toolbar_game_state game_state                = toolbar_game_state::stopped;
    bool               render_properties_changed = false;
    ImGuiAxis          axis                      = ImGuiAxis_X;
};