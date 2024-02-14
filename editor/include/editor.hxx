#pragma once

#include "Kengine/game.hxx"
#include "Kengine/graphics/framebuffer.hxx"
#include "Kengine/lib/lib.hxx"
#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/texture-resource.hxx"

class editor : public Kengine::game
{
public:
    static editor* instance;
    static void    render_imgui();

    editor();

    ~editor() override;
    void on_start() override;
    void on_event(Kengine::event::game_event) override;
    void on_update(int delta_ms) override;
    void on_render(int delta_ms) override;

    on_imgui_render* get_imgui_render() override;

    bool load_game();

    game*               current_game;
    Kengine::lib_handle game_lib;
    bool                need_reload;
    on_imgui_render*    game_imgui;

    Kengine::res_ptr<Kengine::texture_resource>      game_texture_res;
    Kengine::res_ptr<Kengine::renderbuffer_resource> game_renderbuffer_res;
    Kengine::res_ptr<Kengine::framebuffer_resource>  game_framebuffer_res;
    Kengine::graphics::framebuffer                   game_framebuffer;
};