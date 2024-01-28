#include "editor.hxx"

#include "Kengine/engine.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"

#include "imgui.h"

#include <filesystem>

static char game_lib_path[100] = "";
static char base_lib_path[100] = "";
static char temp_lib_path[]    = "./temp.dll";

editor* editor::instance = nullptr;

void editor::render_imgui()
{
    static bool show_open_game_wnd = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open game", NULL, &show_open_game_wnd);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (show_open_game_wnd)
    {
        ImGui::Begin("Open game");
        ImGui::InputText(
            "Path to game lib", game_lib_path, sizeof(game_lib_path));
        ImGui::InputText("Base path", base_lib_path, sizeof(base_lib_path));
        if (ImGui::Button("Load"))
        {
            instance->need_reload = true;
        }
        ImGui::End();
    }

    ImGui::Begin("Game");

    ImGui::Image(reinterpret_cast<ImTextureID>(
                     instance->game_framebuffer.get_color_texture_id()),
                 { 800, 600 });

    ImGui::End();

    if (instance->game_imgui)
        instance->game_imgui();
}

bool editor::load_game()
{
    if (current_game)
    {
        delete current_game;
        Kengine::unload_lib(game_lib);
        game_lib = nullptr;
    }

    using namespace std::filesystem;
    if (exists(temp_lib_path))
    {
        std::error_code remove_error;
        if (!remove(temp_lib_path, remove_error))
        {
            KENGINE_ERROR("Failed to remove temp lib [{}]. Error: {}",
                          temp_lib_path,
                          remove_error.message());
            return false;
        }
    }

    try
    {
        copy_file(game_lib_path, temp_lib_path);
    }
    catch (const std::exception* ex)
    {
        KENGINE_ERROR("Failed to copy from [{}] to [{}]: {}",
                      game_lib_path,
                      temp_lib_path,
                      ex->what());
        return false;
    }

    game_lib = Kengine::load_lib(temp_lib_path);

    if (game_lib == nullptr)
    {
        KENGINE_ERROR("Failed to load game lib.");
        return false;
    }

    Kengine::function_ptr create_game_func_ptr =
        Kengine::load_function(game_lib, "create_game");

    if (create_game_func_ptr == nullptr)
    {
        KENGINE_ERROR("Failed to load function [create_game] from [{}].",
                      temp_lib_path);
        return false;
    }

    using create_game_ptr = decltype(&create_game);

    create_game_ptr create_game_func =
        reinterpret_cast<create_game_ptr>(create_game_func_ptr);

    Kengine::graphics::push_framebuffer(game_framebuffer);

    game* new_game = create_game_func();

    if (new_game == nullptr)
    {
        KENGINE_ERROR("Failed to create game");
        return false;
    }

    current_game = new_game;

    if (strlen(base_lib_path) > 0)
        Kengine::file_manager::set_base_path(base_lib_path);
    else
    {
        std::filesystem::path base_path(game_lib_path);
        base_path = base_path.parent_path();
        Kengine::file_manager::set_base_path(base_path);
    }

    game_imgui = current_game->get_imgui_render();

    current_game->on_start();
    Kengine::graphics::pop_framebuffer();

    return true;
};

editor::~editor()
{
    game_framebuffer      = Kengine::graphics::framebuffer();
    game_texture_res      = nullptr;
    game_renderbuffer_res = nullptr;
    game_framebuffer_res  = nullptr;
}

void editor::on_start()
{
    game_texture_res = Kengine::make_resource<Kengine::texture_resource>(
        Kengine::ivec2(1366, 768),
        Kengine::texture_format::rgba,
        std::string_view("game_texture"));
    game_renderbuffer_res =
        Kengine::make_resource<Kengine::renderbuffer_resource>(
            Kengine::ivec2(1366, 768),
            Kengine::texture_internal_format::depth24_stencil8,
            std::string_view("game_renderbuffer"));
    game_framebuffer_res =
        Kengine::make_resource<Kengine::framebuffer_resource>(
            Kengine::static_resource_cast<Kengine::resource>(game_texture_res),
            Kengine::static_resource_cast<Kengine::resource>(
                game_renderbuffer_res),
            Kengine::vec4{ 0.0, 0.0, 0.0, 0.0 },
            std::string_view("game_framebuffer"));

    game_framebuffer = Kengine::graphics::framebuffer(game_framebuffer_res);
}

void editor::on_event(Kengine::event::game_event event)
{
    if (current_game)
        current_game->on_event(event);
}

void editor::on_update(int delta_ms)
{
    if (current_game)
        current_game->on_update(delta_ms);

    if (need_reload)
    {
        load_game();
        need_reload = false;
    }
}

void editor::on_render(int delta_ms)
{
    if (current_game)
    {
        Kengine::graphics::push_framebuffer(game_framebuffer);
        current_game->on_render(delta_ms);
        Kengine::graphics::pop_framebuffer();
    }
}

on_imgui_render* editor::get_imgui_render()
{
    return &render_imgui;
}

Kengine::game* create_game()
{
    editor::instance = new editor();
    return editor::instance;
}

int main()
{
    Kengine::configuration wnd_startup_options{};
    wnd_startup_options.set_setting("options", "fullscreen", false);
    Kengine::window::override_startup_options(wnd_startup_options);

    if (Kengine::run(&create_game, "Kengine Editor"))
        return EXIT_SUCCESS;
    else
        return EXIT_FAILURE;
}