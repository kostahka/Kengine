#pragma once

#include "Kengine/string/string-id.hxx"

#include <memory>
#include <string_view>
#include <unordered_map>


namespace Kengine
{
    class scene;

    class render_system
    {
    public:
        render_system(std::string_view name);

        virtual void on_render(scene&, int delta_ms) = 0;

        inline string_id get_name_id() const { return name_id; }

    private:
        string_id name_id;
    };

    class update_system
    {
    public:
        update_system(std::string_view name);

        virtual void on_update(scene&, int delta_ms) = 0;

        inline string_id get_name_id() const { return name_id; }

    private:
        string_id name_id;
    };

    class system_container
    {
    public:
        system_container() = default;

        void register_render_system(std::shared_ptr<render_system>);
        void register_render_system(std::shared_ptr<update_system>);
        std::shared_ptr<render_system> get_render_system(string_id name_id);
        std::shared_ptr<update_system> get_update_system(string_id name_id);

    private:
        std::unordered_map<string_id, std::shared_ptr<render_system>>
            render_systems;
        std::unordered_map<string_id, std::shared_ptr<update_system>>
            update_systems;
    };
} // namespace Kengine