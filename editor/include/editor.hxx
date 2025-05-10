#pragma once

#include "toolbar-wnd.hxx"

#include "Kengine/game.hxx"
#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/lib/lib.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/texture-resource.hxx"
#include "Kengine/scene/scene.hxx"
#include <efsw/efsw.hpp>
#include <memory>

enum class editor_game_mode
{
    play_mode,
    edit_mode
};

class editor : public Kengine::game
{
public:
    static editor* instance;
    static void    render_imgui();
    static void    build_layout();

    editor();

    ~editor() override;
    void on_start() override;
    void on_event(const Kengine::event::game_event&) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;

    void set_game_scene(Kengine::string_id, bool recreate_camera = true);
    void set_game_scene(std::filesystem::path, bool recreate_camera = true);

    void set_scene_camera(entt::entity);

    void invalid_scene_render();

    on_imgui_render* get_imgui_render() override;

    bool load_game();

    game*               current_game = nullptr;
    Kengine::lib_handle game_lib     = nullptr;
    on_imgui_render*    game_imgui   = nullptr;

    Kengine::res_ptr<Kengine::texture_resource>      game_texture_res = nullptr;
    Kengine::res_ptr<Kengine::renderbuffer_resource> game_renderbuffer_res =
        nullptr;
    Kengine::res_ptr<Kengine::framebuffer_resource> game_framebuffer_res =
        nullptr;
    Kengine::graphics::framebuffer game_framebuffer;

    bool scene_render_valid = true;
    bool scene_update_valid = true;

    entt::entity selected_entity = entt::null;

    Kengine::res_ptr<Kengine::resource> current_res = nullptr;

    static Kengine::ivec2 game_viewport_size;

    Kengine::graphics::camera edit_camera;
    Kengine::vec2             edit_camera_pos{ 0, 0 };
    float                     edit_camera_angle{ 0 };

    class game_lib_file : public efsw::FileWatchListener
    {
    public:
        void handleFileAction(efsw::WatchID      watchid,
                              const std::string& dir,
                              const std::string& filename,
                              efsw::Action       action,
                              std::string        oldFilename = "") override;

        bool          need_reload = false;
        efsw::WatchID watch_id    = 0;
        std::string   file_name;
    } game_lib_file;

    class game_mode_info : public toolbar_game_state_listener
    {
    public:
        void game_state_changed(toolbar_game_state old_state,
                                toolbar_game_state new_state) override final;

        editor_game_mode mode              = editor_game_mode::edit_mode;
        bool             need_reload_scene = false;
    };

    std::shared_ptr<game_mode_info> game_mode =
        std::make_shared<game_mode_info>();
};