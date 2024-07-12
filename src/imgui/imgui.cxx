#include "Kengine/imgui/imgui.hxx"
#include "imgui.hxx"

#include "../window/window.hxx"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <SDL3/SDL_events.h>

#ifdef KENGINE_IMGUI
namespace Kengine::imgui
{
    void default_imgui_render() {}

    on_imgui_render* render_imgui = default_imgui_render;

    void initialize()
    {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        ImGui_ImplSDL3_InitForOpenGL(window::window, window::context);
        ImGui_ImplOpenGL3_Init("#version 300 es");
        SDL_StartTextInput(window::window);
    }

    void shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
        SDL_StopTextInput(window::window);
    }

    void draw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        render_imgui();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };

    void registrate_imgui_render(on_imgui_render* func)
    {
        render_imgui = func;
    }

    void set_cursor_visible(bool visible)
    {
        if (visible)
        {
            ImGui::GetIO().ConfigFlags ^= ImGuiConfigFlags_NoMouseCursorChange;
        }
        else
        {
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        }
    }
} // namespace Kengine::imgui
#endif