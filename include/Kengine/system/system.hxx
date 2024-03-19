#pragma once

#include "Kengine/string/string-id.hxx"

#include <memory>
#include <string_view>
#include <unordered_map>

namespace Kengine
{
    class scene;

    enum system_type
    {
        system_render_type = 1,
        system_update_type = 2,
    };

    class system : public serializable
    {
    public:
        system(std::string_view name,
               uint8_t type_flags = system_render_type | system_update_type);

        virtual void on_create(scene&);

        virtual void on_render(scene&, int delta_ms);
        virtual void on_update(scene&, int delta_ms);

        inline string_id get_name_id() const { return name_id; }

        inline uint8_t get_type_flags() const { return type_flags; }

    private:
        uint8_t   type_flags;
        string_id name_id;
    };

    typedef std::shared_ptr<system> (*create_system_fp)();

    class system_container
    {
    public:
        system_container();

        void             register_system(string_id name_id, create_system_fp);
        create_system_fp get_system_factory(string_id name_id);

        template <typename SystemType>
        void register_system(std::string_view name)
        {
            static_assert(std::is_base_of_v<system, SystemType>,
                          "SystemType only inhertis system");

            create_system_fp system_factory = []()
            {
                return static_cast<std::shared_ptr<system>>(
                    std::make_shared<SystemType>());
            };
            auto name_id = hash_string(name.data());

            register_system(name_id, system_factory);
        }

        inline const auto& get_system_factories() const
        {
            return system_factories;
        }

    private:
        std::unordered_map<string_id, create_system_fp> system_factories;
    };
} // namespace Kengine