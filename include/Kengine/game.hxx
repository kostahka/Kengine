#pragma once

#include <string>

#include "Kengine/components/component-container.hxx"
#include "Kengine/event/event.hxx"
#include "Kengine/imgui/imgui.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/system/system.hxx"
#include "Kengine/window/window.hxx"

#include "glm/glm.hpp"

#include <filesystem>
#include <unordered_map>

namespace Kengine
{
    class game : public system_container, public component_container
    {
    public:
        game();

        void set_current_scene(string_id);

        inline void set_current_scene(std::string_view scene_name)
        {
            auto name_id = hash_string(scene_name.data());
            set_current_scene(name_id);
        }

        void load_scene_links();
        void save_scene_links();
        void add_scene_link(std::string_view name, std::filesystem::path path);
        void remove_scene_link(string_id);

        void save_scene(string_id scene_id, const scene& sc);

        virtual ~game()                          = default;
        virtual void on_start()                  = 0;
        virtual void on_event(event::game_event) = 0;
        virtual void on_update(int delta_ms)     = 0;
        virtual void on_render(int delta_ms)     = 0;

        virtual on_imgui_render* get_imgui_render() { return nullptr; }

        inline const auto get_scene_links() const { return scene_links; }

        inline scene& get_current_scene() { return *current_scene; }

        inline string_id get_current_scene_id() const
        {
            return current_scene_id;
        }

    private:
        std::unordered_map<string_id, std::filesystem::path> scene_links;
        std::shared_ptr<scene>                               current_scene;
        string_id                                            current_scene_id;
    };

} // namespace Kengine

typedef Kengine::game*   create_game_func();
extern "C" GAME_DECLSPEC Kengine::game* create_game();