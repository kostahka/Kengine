#include "game-wnd.hxx"

#include "imgui.h"

#include <math.h>

game_wnd::game_wnd() {}

void game_wnd::display()
{
    ImGui::PushID(this);

    auto current_game = editor::instance->current_game;

    {
        ImGui::Begin("Game");

        auto window_height = ImGui::GetWindowHeight();
        auto window_width  = ImGui::GetWindowWidth();

        auto game_image_height = window_height - 50;
        auto game_image_width  = editor::game_viewport_size.x *
                                game_image_height /
                                editor::game_viewport_size.y;

        ImGui::SetCursorPosX((window_width - game_image_width) / 2);
        ImGui::SetCursorPosY((window_height - game_image_height) / 2);

        ImGui::Image(
            reinterpret_cast<ImTextureID>(
                editor::instance->game_framebuffer.get_color_texture_id()),
            { game_image_width, game_image_height },
            { 0, 1 },
            { 1, 0 });

        if (ImGui::IsWindowHovered())
        {
            auto drag_camera = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
            auto scale_camera = -ImGui::GetIO().MouseWheel;

            auto current_camera_height =
                std::abs(editor::instance->edit_camera.get_height());

            if (scale_camera != 0)
            {
                editor::instance->edit_camera.set_height(
                    current_camera_height +
                    scale_camera * current_camera_height / 10.f + 0.0001f);
            }

            editor::instance->edit_camera_pos.x -=
                drag_camera.x / game_image_width * current_camera_height;
            editor::instance->edit_camera_pos.y +=
                drag_camera.y / game_image_height * current_camera_height;

            if (drag_camera.x != 0 || drag_camera.y != 0 || scale_camera != 0)
            {
                editor::instance->invalid_scene_render();
            }
        }

        ImGui::Text("Edit camera:");
        ImGui::SameLine();
        ImGui::Text("height: %f", editor::instance->edit_camera.get_height());
        ImGui::SameLine();
        ImGui::Text("position: (%f, %f)",
                    editor::instance->edit_camera_pos.x,
                    editor::instance->edit_camera_pos.y);

        ImGui::End();
    }

    ImGui::PopID();
}