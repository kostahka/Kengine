#include "toolbar-wnd.hxx"
#include "box2d/b2_draw.h"
#include "imgui.h"
#include "imgui_internal.h"

toolbar_wnd::toolbar_wnd() {}

void toolbar_wnd::display(uint32_t& b2_debug_draw_flags)
{
    render_properties_changed = false;

    ImGui::PushID(this);

    {
        ImGuiWindowClass window_class{};
        window_class.DockingAllowUnclassed = true;
        window_class.DockNodeFlagsOverrideSet |=
            ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_HiddenTabBar |
            ImGuiDockNodeFlags_NoDockingOverMe |
            ImGuiDockNodeFlags_NoDockingSplit |
            ImGuiDockNodeFlags_NoDockingOverOther;

        window_class.DockNodeFlagsOverrideSet |=
            axis == ImGuiAxis_X ? ImGuiDockNodeFlags_NoResizeY
                                : ImGuiDockNodeFlags_NoResizeX;

        ImGui::SetNextWindowClass(&window_class);

        ImGui::Begin(window_name,
                     NULL,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

        const ImVec2 icon_size(15, 15);
        ImFont*      font            = ImGui::GetFont();
        auto         prev_font_scale = font->Scale;
        auto         prev_font_size  = font->FontSize;
        font->Scale                  = icon_size.x / prev_font_size;
        ImGui::PushFont(font);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2);

        ImGuiDockNode* node = ImGui::GetWindowDockNode();
        if (node)
        {
            if (node->ParentNode &&
                node->ParentNode->SplitAxis != ImGuiAxis_None)
            {
                axis = (ImGuiAxis)(node->ParentNode->SplitAxis ^ 1);
            }

            ImGuiStyle&     style     = ImGui::GetStyle();
            const ImGuiAxis axis_perp = (ImGuiAxis)(axis ^ 1);
            const float     dock_size = (style.WindowPadding[axis_perp] +
                                     style.FramePadding[axis_perp]) *
                                        2.f +
                                    icon_size[axis_perp];
            node->WantLockSizeOnce = true;
            node->Size[axis_perp] = node->SizeRef[axis_perp] = dock_size;
        }

        const auto child_size =
            (ImGui::GetWindowSize().x - ImGui::GetStyle().FramePadding.x * 2) /
            3;

        {
            ImGui::BeginChild("Others", { child_size, 0 }, 0);
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("Game modes", { child_size, 0 }, 0);

            static const char* play_btn_text  = "Play";
            static const char* pause_btn_text = "Pause";
            static const char* stop_btn_text  = "Stop";
            const auto buttons_width = ImGui::CalcTextSize(play_btn_text).x +
                                       ImGui::CalcTextSize(pause_btn_text).x +
                                       ImGui::CalcTextSize(stop_btn_text).x +
                                       ImGui::GetStyle().FramePadding.x * 6;

            const auto buttons_pos_x =
                (ImGui::GetWindowSize().x - buttons_width) / 2;

            ImGui::SetCursorPosX(buttons_pos_x);

            const auto play_btn_color = play_mode == game_play_mode::play
                                            ? ImGuiCol_ButtonActive
                                            : ImGuiCol_Button;
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImGui::GetStyle().Colors[play_btn_color]);

            if (ImGui::Button(play_btn_text))
            {
                play_mode = game_play_mode::play;
            }
            ImGui::PopStyleColor();

            ImGui::SameLine();

            const auto pause_btn_color = play_mode == game_play_mode::paused
                                             ? ImGuiCol_ButtonActive
                                             : ImGuiCol_Button;
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImGui::GetStyle().Colors[pause_btn_color]);

            ImGui::BeginDisabled(play_mode != game_play_mode::play);
            if (ImGui::Button(pause_btn_text))
            {
                play_mode = game_play_mode::paused;
            }
            ImGui::EndDisabled();
            ImGui::PopStyleColor();

            ImGui::SameLine();

            ImGui::BeginDisabled(play_mode == game_play_mode::stopped);
            if (ImGui::Button(stop_btn_text))
            {
                play_mode = game_play_mode::stopped;
            }
            ImGui::EndDisabled();

            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("Properties", { child_size, 0 }, 0);

            if (ImGui::Button("GUI"))
            {
                ImGui::OpenPopup("GUI_Properties");
            }
            if (ImGui::BeginPopup("GUI_Properties"))
            {
                if (ImGui::MenuItem("GUI debug draw", {}, &gui_debug_draw))
                {
                    render_properties_changed = true;
                }
                ImGui::EndPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Physics"))
            {
                ImGui::OpenPopup("Physics_Properties");
            }
            if (ImGui::BeginPopup("Physics_Properties"))
            {
                if (ImGui::MenuItem(
                        "Physics debug draw", {}, &physics_debug_draw))
                {
                    render_properties_changed = true;
                }

                bool b2_debug_shape = b2_debug_draw_flags & b2Draw::e_shapeBit;
                bool b2_debug_aabb  = b2_debug_draw_flags & b2Draw::e_aabbBit;
                bool b2_debug_center_of_mass =
                    b2_debug_draw_flags & b2Draw::e_centerOfMassBit;
                bool b2_debug_joint = b2_debug_draw_flags & b2Draw::e_jointBit;
                bool b2_debug_pair  = b2_debug_draw_flags & b2Draw::e_pairBit;

                if (ImGui::MenuItem("Draw physics shapes", {}, &b2_debug_shape))
                {
                    b2_debug_draw_flags =
                        b2_debug_draw_flags ^ b2Draw::e_shapeBit;
                    render_properties_changed = true;
                }
                if (ImGui::MenuItem("Draw physics aabb", {}, &b2_debug_aabb))
                {
                    b2_debug_draw_flags =
                        b2_debug_draw_flags ^ b2Draw::e_aabbBit;
                    render_properties_changed = true;
                }
                if (ImGui::MenuItem("Draw physics center of mass",
                                    {},
                                    &b2_debug_center_of_mass))
                {
                    b2_debug_draw_flags =
                        b2_debug_draw_flags ^ b2Draw::e_centerOfMassBit;
                    render_properties_changed = true;
                }
                if (ImGui::MenuItem("Draw physics joints", {}, &b2_debug_joint))
                {
                    b2_debug_draw_flags =
                        b2_debug_draw_flags ^ b2Draw::e_jointBit;
                    render_properties_changed = true;
                }
                if (ImGui::MenuItem("Draw physics pairs", {}, &b2_debug_pair))
                {
                    b2_debug_draw_flags =
                        b2_debug_draw_flags ^ b2Draw::e_pairBit;
                    render_properties_changed = true;
                }

                ImGui::EndPopup();
            }

            ImGui::EndChild();
        }

        ImGui::PopStyleVar(2);
        font->Scale = prev_font_scale;
        ImGui::PopFont();
        ImGui::End();
    }

    ImGui::PopID();
}