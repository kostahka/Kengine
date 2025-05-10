#include "editor.hxx"

#include "Kengine/io/file-manager.hxx"
#include "assets-browser.hxx"
#include "efsw/efsw.hpp"
#include "game-properties-wnd.hxx"
#include "game-wnd.hxx"
#include "log-wnd.hxx"
#include "object-properties-wnd.hxx"
#include "resource-wnd.hxx"
#include "scene-objects-wnd.hxx"
#include "scene-properties-wnd.hxx"

#include "Kengine/configuration/configuration-file.hxx"
#include "Kengine/engine.hxx"
#include "Kengine/graphics/b2GLDraw.hxx"
#include "Kengine/graphics/graphics.hxx"
#include "Kengine/graphics/gui-draw-debug.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/scene/scene-manager.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui-filebrowser/imfilebrowser.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "toolbar-wnd.hxx"

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
game_wnd              game_window{};
log_wnd               log_window{};
toolbar_wnd           toolbar_window{};

Kengine::configuration_file editor_config{ "kengine-editor" };

editor* editor::instance = nullptr;

static ImGuiID main_dockspace_id  = 1;
static bool    layout_initialized = false;

b2GLDraw                b2_debug_draw{};
Kengine::gui_draw_debug gui_debug_draw{};

Kengine::ivec2 editor::game_viewport_size{ 800, 600 };

static efsw::FileWatcher file_watcher{};

void editor::game_lib_file::handleFileAction(efsw::WatchID      watchid,
                                             const std::string& dir,
                                             const std::string& filename,
                                             efsw::Action       action,
                                             std::string        oldFilename)
{
    if (action != efsw::Action::Delete)
    {
        if (filename == file_name)
        {
            need_reload = true;
        }
    }
}

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

        if (ImGui::BeginMenu("Layout"))
        {
            if (ImGui::MenuItem("Default"))
            {
                layout_initialized = false;
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

        instance->game_lib_file.need_reload = true;
        base_assets_file_browser.ClearSelected();
    }

    // ImGui::ShowDemoWindow();
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::DockSpaceOverViewport(main_dockspace_id, viewport);

        if (!layout_initialized)
        {
            build_layout();
        }

        game_lib_file_browser.Display();
        base_assets_file_browser.Display();

        a_browser.display();
        game_properties_window.display();
        scene_properties_window.display();
        scene_objects_window.display();
        object_properties_window.display();
        resource_window.display();
        game_window.display();
        log_window.display();

        {
            auto b2_debug_draw_flags = b2_debug_draw.GetFlags();
            toolbar_window.display(b2_debug_draw_flags);

            if (toolbar_window.is_render_properties_changed())
            {
                b2_debug_draw.SetFlags(b2_debug_draw_flags);
                editor::instance->invalid_scene_render();
            }
        }
    }

    if (instance->game_imgui)
        instance->game_imgui();
}

bool editor::load_game()
{
    if (current_game)
    {
        KENGINE_INFO("Unloading game library");
        delete current_game;
        Kengine::unload_lib(game_lib);
        game_lib = nullptr;
    }

    file_watcher.removeWatch(game_lib_file.watch_id);

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

    KENGINE_INFO("Loading game library: {}", game_lib_path.string().c_str());
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

    std::filesystem::path game_lib_dir      = game_lib_path.parent_path();
    std::filesystem::path game_lib_filename = game_lib_path.filename();

    game_lib_file.file_name = game_lib_filename.string();
    file_watcher.addWatch(game_lib_dir.string(), &game_lib_file);

    return true;
};

void editor::build_layout()
{
    ImGui::DockBuilderRemoveNodeChildNodes(main_dockspace_id);

    ImGuiID node1;
    ImGuiID node2;
    ImGui::DockBuilderSplitNode(
        main_dockspace_id, ImGuiDir_Left, 0.8f, &node1, &node2);

    ImGuiID node11;
    ImGuiID node12;
    ImGui::DockBuilderSplitNode(node1, ImGuiDir_Down, 0.3f, &node11, &node12);

    ImGuiID node121;
    ImGuiID node122;
    ImGui::DockBuilderSplitNode(
        node12, ImGuiDir_Left, 0.2f, &node121, &node122);

    ImGuiID node1221;
    ImGuiID node1222;
    ImGui::DockBuilderSplitNode(
        node122, ImGuiDir_Down, 0.8f, &node1221, &node1222);

    ImGuiID node111;
    ImGuiID node112;
    ImGui::DockBuilderSplitNode(
        node11, ImGuiDir_Left, 0.5f, &node111, &node112);

    ImGuiID node21;
    ImGuiID node22;
    ImGui::DockBuilderSplitNode(node2, ImGuiDir_Down, 0.4f, &node21, &node22);

    ImGui::DockBuilderDockWindow(resource_window.window_name, node21);
    ImGui::DockBuilderDockWindow(game_properties_window.window_name, node22);
    ImGui::DockBuilderDockWindow(scene_properties_window.window_name, node22);
    ImGui::DockBuilderDockWindow(object_properties_window.window_name, node22);
    ImGui::DockBuilderDockWindow(a_browser.window_name, node111);
    ImGui::DockBuilderDockWindow(log_window.window_name, node112);
    ImGui::DockBuilderDockWindow(scene_objects_window.window_name, node121);
    ImGui::DockBuilderDockWindow(toolbar_window.window_name, node1222);
    ImGui::DockBuilderDockWindow(game_window.window_name, node1221);

    layout_initialized = true;
}

editor::editor()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    editor_config.load();
    layout_initialized = Kengine::file_manager::file_exists("imgui.ini");

    game_lib_path =
        editor_config.get_setting("game", "lib_path", std::filesystem::path());
    assets_base_path = editor_config.get_setting(
        "game", "assets_base_path", std::filesystem::path());

    if (!game_lib_path.empty())
    {
        a_browser.assets_file_browser.SetPwd(assets_base_path);
        game_lib_file.need_reload = true;
    }

    edit_camera.set_projection(1, -100, 100);

    log_window.initialize();
};

editor::~editor()
{
    if (current_game)
    {
        delete current_game;
    }
    game_framebuffer      = Kengine::graphics::framebuffer();
    game_texture_res      = nullptr;
    game_renderbuffer_res = nullptr;
    game_framebuffer_res  = nullptr;

    editor_config.set_setting("game", "lib_path", game_lib_path);
    editor_config.set_setting("game", "assets_base_path", assets_base_path);
    editor_config.save();

    gui_debug_draw.destroy();
    b2_debug_draw.Destroy();
}

void editor::set_game_scene(Kengine::string_id sc_link)
{
    edit_camera = Kengine::graphics::camera();
    edit_camera.set_projection(1, -100, 100);
    edit_camera_pos   = { 0, 0 };
    edit_camera_angle = 0;

    get_current_scene().clear_resources();
    current_game->set_current_scene(sc_link);
    invalid_scene_render();
    current_game->get_current_scene().set_camera(&edit_camera);
}

void editor::set_game_scene(std::filesystem::path sc_path)
{
    edit_camera = Kengine::graphics::camera();
    edit_camera.set_projection(1, -100, 100);
    edit_camera_pos   = { 0, 0 };
    edit_camera_angle = 0;

    get_current_scene().clear_resources();
    current_game->set_current_scene(sc_path);
    current_game->save_scene_links();
    invalid_scene_render();
    current_game->get_current_scene().set_camera(&edit_camera);
}

void editor::set_scene_camera(entt::entity ent)
{
    current_game->get_current_scene().set_camera(ent);
    current_game->get_current_scene().set_camera(&edit_camera);
}

void editor::on_start()
{
    game_texture_res = Kengine::make_resource<Kengine::texture_resource>(
        game_viewport_size,
        Kengine::texture_format::rgba,
        std::string_view("game_texture"));
    game_renderbuffer_res =
        Kengine::make_resource<Kengine::renderbuffer_resource>(
            game_viewport_size,
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
    gui_debug_draw.create();

    file_watcher.watch();
}

void editor::on_event(const Kengine::event::game_event& event)
{
    if (current_game)
    {
        current_game->on_event(event);
        current_game->get_current_scene().on_event(event);
        invalid_scene_render();
    }
}

void editor::on_update(int delta_ms)
{

    const Kengine::vec3 eye    = { edit_camera_pos.x, edit_camera_pos.y, 0 };
    const Kengine::vec3 center = { eye.x, eye.y, eye.z - 1 };
    const Kengine::vec3 up     = { std::sin(edit_camera_angle),
                                   std::cos(edit_camera_angle),
                                   0 };

    Kengine::mat4 edit_camera_view = glm::lookAtRH(eye, center, up);

    edit_camera.view = edit_camera_view;

    if ((play_mode || !scene_update_valid) && current_game)
    {
        current_game->on_update(delta_ms);
        current_game->get_current_scene().on_update(delta_ms);

        scene_update_valid = true;
    }

    if (game_lib_file.need_reload)
    {
        get_current_scene().clear_resources();
        load_game();
        game_lib_file.need_reload = false;
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

        if (!edit_camera.is_projection_valid())
        {
            edit_camera.calculate_projection();
        }

        current_game->on_render(delta_ms);
        current_game->get_current_scene().on_render(delta_ms);
        if (toolbar_window.physics_debug_draw)
        {
            current_game->get_current_scene().get_world().SetDebugDraw(
                &b2_debug_draw);
            current_game->get_current_scene().get_world().DebugDraw();
            b2_debug_draw.Draw();
        }
        if (toolbar_window.physics_debug_draw)
        {
            gui_debug_draw.draw(current_game->get_current_scene());
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