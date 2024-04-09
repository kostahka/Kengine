#pragma once

#include "Kengine/game.hxx"
#include "Kengine/graphics/camera.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/lib/lib.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/texture-resource.hxx"
#include "Kengine/scene/scene.hxx"

class editor : public Kengine::game
{
public:
    static editor* instance;
    static void    render_imgui();

    editor();

    ~editor() override;
    void on_start() override;
    void on_event(const Kengine::event::game_event&) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;

    void set_game_scene(Kengine::string_id);
    void set_game_scene(std::filesystem::path);

    void set_scene_camera(entt::entity);

    void invalid_scene_render();

    on_imgui_render* get_imgui_render() override;

    bool load_game();

    game*               current_game = nullptr;
    Kengine::lib_handle game_lib     = nullptr;
    bool                need_reload  = false;
    on_imgui_render*    game_imgui   = nullptr;

    Kengine::res_ptr<Kengine::texture_resource>      game_texture_res = nullptr;
    Kengine::res_ptr<Kengine::renderbuffer_resource> game_renderbuffer_res =
        nullptr;
    Kengine::res_ptr<Kengine::framebuffer_resource> game_framebuffer_res =
        nullptr;
    Kengine::graphics::framebuffer game_framebuffer;

    bool play_mode          = false;
    bool scene_render_valid = true;
    bool scene_update_valid = true;

    entt::entity selected_entity = entt::null;

    Kengine::res_ptr<Kengine::resource> current_res = nullptr;

    static Kengine::ivec2 game_viewport_size;

    Kengine::graphics::camera edit_camera;
    Kengine::vec2             edit_camera_pos{ 0, 0 };
    float                     edit_camera_angle{ 0 };
};