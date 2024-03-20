#include "editor.hxx"

#include "assets-browser.hxx"
#include "game-properties-wnd.hxx"
#include "object-properties-wnd.hxx"
#include "resource-wnd.hxx"
#include "scene-objects-wnd.hxx"
#include "scene-properties-wnd.hxx"

#include "Kengine/configuration/configuration-file.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/graphics/b2GLDraw.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui-filebrowser/imfilebrowser.h"
#include "imgui.h"

#include "box2d/box2d.h"

#include <filesystem>

static std::filesystem::path game_lib_path{ "" };
static std::filesystem::path assets_base_path{ "" };
static std::filesystem::path temp_lib_path{ "./temp.dll" };

ImGui::FileBrowser game_lib_file_browser{ "Select game lib" };
ImGui::FileBrowser base_assets_file_browser{
    "Select assets root folder",
    ImGuiFileBrowserFlags_SelectDirectory |
        ImGuiFileBrowserFlags_HideRegularFiles
};

game_properties_wnd   game_properties_window{};
assets_browser        a_browser{};
scene_properties_wnd  scene_properties_window{};
scene_objects_wnd     scene_objects_window{};
object_properties_wnd object_properties_window{};
resource_wnd          resource_window{};

Kengine::configuration_file editor_config{ "kengine-editor" };

editor* editor::instance = nullptr;

b2GLDraw b2_debug_draw{};
bool     physics_debug_draw = false;

void editor::render_imgui()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open game", NULL, false))
            {
                game_lib_file_browser.Open();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (game_lib_file_browser.HasSelected())
    {
        game_lib_path = game_lib_file_browser.GetSelected();

        base_assets_file_browser.Open();

        game_lib_file_browser.ClearSelected();
    }

    if (base_assets_file_browser.HasSelected())
    {
        assets_base_path = base_assets_file_browser.GetSelected();

        a_browser.assets_file_browser.SetPwd(assets_base_path);

        instance->need_reload = true;
        base_assets_file_browser.ClearSelected();
    }

    ImGui::ShowDemoWindow();
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::Begin("GUI",
                     nullptr,
                     ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);
        ImGui::End();

        game_lib_file_browser.Display();
        base_assets_file_browser.Display();

        a_browser.display();
        game_properties_window.display();
        scene_properties_window.display();
        scene_objects_window.display();
        object_properties_window.display();
        resource_window.display();

        {
            ImGui::Begin("Properties");

            ImGui::Checkbox("Physics debug draw", &physics_debug_draw);

            ImGui::End();
        }

        {
            ImGui::Begin("Game");

            auto windowHeight = ImGui::GetWindowHeight();
            auto windowWidth  = ImGui::GetWindowWidth();

            auto gameImageHeight = windowHeight - 50;
            auto gameImageWidth  = 800 * gameImageHeight / 600;

            ImGui::SetCursorPosX((windowWidth - gameImageWidth) / 2);
            ImGui::SetCursorPosY((windowHeight - gameImageHeight) / 2);

            ImGui::Image(reinterpret_cast<ImTextureID>(
                             instance->game_framebuffer.get_color_texture_id()),
                         { gameImageWidth, gameImageHeight },
                         { 0, 1 },
                         { 1, 0 });

            ImGui::End();
        }
    }

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
                          temp_lib_path.string().c_str(),
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
                      game_lib_path.string().c_str(),
                      temp_lib_path.string().c_str(),
                      ex->what());
        return false;
    }

    game_lib = Kengine::load_lib(temp_lib_path.string().c_str());

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
                      temp_lib_path.string().c_str());
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

    if (!assets_base_path.empty())
        Kengine::scene_manager::set_assets_base_folder(assets_base_path);
    else
    {
        assets_base_path = game_lib_path.parent_path();
        Kengine::scene_manager::set_assets_base_folder(assets_base_path);
    }

    current_game->load_scene_links();

    game_imgui = current_game->get_imgui_render();

    current_game->on_start();
    Kengine::graphics::pop_framebuffer();

    return true;
};

editor::editor()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    editor_config.load();
    game_lib_path =
        editor_config.get_setting("game", "lib_path", std::filesystem::path());
    assets_base_path = editor_config.get_setting(
        "game", "assets_base_path", std::filesystem::path());

    if (!game_lib_path.empty())
    {
        a_browser.assets_file_browser.SetPwd(assets_base_path);
        need_reload = true;
    }
};

editor::~editor()
{
    game_framebuffer      = Kengine::graphics::framebuffer();
    game_texture_res      = nullptr;
    game_renderbuffer_res = nullptr;
    game_framebuffer_res  = nullptr;

    editor_config.set_setting("game", "lib_path", game_lib_path);
    editor_config.set_setting("game", "assets_base_path", assets_base_path);
    editor_config.save();

    b2_debug_draw.Destroy();
}

void editor::set_game_scene(Kengine::string_id sc_link)
{
    get_current_scene().clear_resources();
    current_game->set_current_scene(sc_link);
    invalid_scene_render();
    current_game->get_current_scene().get_world().SetDebugDraw(&b2_debug_draw);
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

    b2_debug_draw.Create();
    b2_debug_draw.SetFlags(b2Draw::e_shapeBit);
}

void editor::on_event(Kengine::event::game_event event)
{
    if (play_mode && current_game)
        current_game->on_event(event);
}

void editor::on_update(int delta_ms)
{
    if ((play_mode || !scene_update_valid) && current_game)
    {
        current_game->on_update(delta_ms);
        current_game->get_current_scene().on_update(delta_ms);

        scene_update_valid = true;
    }

    if (need_reload)
    {
        get_current_scene().clear_resources();
        load_game();
        need_reload = false;
    }
}

void editor::on_render(int delta_ms)
{
    if (current_game &&
        (play_mode || (!scene_render_valid && scene_update_valid)))
    {
        game_framebuffer_res->set_clear_color(
            current_game->get_current_scene().clear_color);

        Kengine::graphics::push_framebuffer(game_framebuffer);
        current_game->on_render(delta_ms);
        current_game->get_current_scene().on_render(delta_ms);
        if (physics_debug_draw)
        {
            current_game->get_current_scene().get_world().DebugDraw();
            b2_debug_draw.Draw();
        }
        Kengine::graphics::pop_framebuffer();

        scene_render_valid = true;
    }
}

void editor::invalid_scene_render()
{
    scene_render_valid = false;
    scene_update_valid = false;
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