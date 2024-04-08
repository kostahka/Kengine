#pragma once

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Kengine
{
    class E_DECLSPEC configuration
    {
        typedef std::unordered_map<std::string, std::string> settings_map;
        typedef std::unordered_map<std::string, settings_map>
            section_settings_map;

    public:
        configuration();
        ~configuration();

        void override_settings_with(configuration other);

        void set_setting(std::string section,
                         std::string setting,
                         std::string value);

        template <typename T>
        void set_setting(std::string section, std::string setting, T value)
        {
            std::stringstream setting_stream;
            setting_stream << value;
            set_setting(section, setting, setting_stream.str());
        }

        std::string get_setting(std::string section,
                                std::string setting,
                                std::string default_value);

        template <typename T>
        T get_setting(std::string section, std::string setting, T default_value)
        {
            T result;

            std::stringstream setting_stream;
            setting_stream << default_value;
            std::string setting_value =
                get_setting(section, setting, setting_stream.str());
            setting_stream.str("");
            setting_stream << setting_value;
            setting_stream >> result;

            return result;
        }

    protected:
        section_settings_map settings;
    };
} // namespace Kengine