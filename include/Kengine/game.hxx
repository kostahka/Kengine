#pragma once

#include <string>

#include "Kengine/event/event.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/scene/scene-link.hxx"
#include "Kengine/system/system.hxx"
#include "Kengine/window/window.hxx"

#include "glm/glm.hpp"

#include <vector>

namespace Kengine
{
    class game : public system_container
    {
    public:
        game();
        void load_scene_links();
        void save_scene_links();
        void add_scene_link(std::string_view name, std::filesystem::path path);

        virtual ~game()                          = default;
        virtual void on_start()                  = 0;
        virtual void on_event(event::game_event) = 0;
        virtual void on_update(int delta_ms)     = 0;
        virtual void on_render(int delta_ms)     = 0;

        virtual on_imgui_render* get_imgui_render() { return nullptr; };

        void set_current_scene(std::shared_ptr<scene>);

        std::vector<scene_link> scene_links;
        std::shared_ptr<scene>  current_scene;
    };

} // namespace Kengine

typedef Kengine::game*   create_game_func();
extern "C" GAME_DECLSPEC Kengine::game* create_game();