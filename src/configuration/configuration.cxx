#include "Kengine/configuration/configuration.hxx"

#include "Kengine/io/file-manager.hxx"

namespace Kengine
{
    configuration::configuration() {}

    configuration::~configuration() {}

    void configuration::override_settings_with(configuration other)
    {
        for (auto& section_settings : other.settings)
        {
            for (auto& setting : section_settings.second)
            {
                settings[section_settings.first][setting.first] =
                    setting.second;
            }
        }
    }

    void configuration::set_setting(std::string section,
                                    std::string setting,
                                    std::string value)
    {
        settings[section][setting] = value;
    }

    std::string configuration::get_setting(std::string section,
                                           std::string setting,
                                           std::string default_value)
    {
        auto f_section = settings.find(section);
        if (f_section != settings.end())
        {
            auto f_setting = f_section->second.find(setting);
            if (f_setting != f_section->second.end())
            {
                return f_setting->second;
            }
        }

        settings[section][setting] = default_value;

        return default_value;
    }

} // namespace Kengine