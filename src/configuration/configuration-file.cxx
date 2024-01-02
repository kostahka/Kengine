#include "Kengine/configuration/configuration-file.hxx"

#include "Kengine/io/file-manager.hxx"

namespace Kengine
{
    configuration_file::configuration_file(std::string_view name)
        : name(name)
        , configuration()
    {
        configuration_path += "./";
        configuration_path += name;
        configuration_path += ".ini";
    }

    configuration_file::~configuration_file() {}

    bool configuration_file::load()
    {
        if (!file_manager::file_exists(configuration_path))
            return false;

        auto configuration_file = file_manager::load_file(configuration_path);
        if (!configuration_file)
            return false;

        std::istream c_stream(configuration_file.get());

        std::string current_section = "";

        std::string line;
        while (std::getline(c_stream, line))
        {
            if (line.length() > 0 && line.at(0) != '#' && line.at(0) != '@')
            {
                if (line.at(0) == '[' && line.at(line.length() - 1) == ']')
                {
                    current_section = line.substr(1, line.length() - 2);
                }
                else
                {
                    auto separator_pos = line.find_first_of('=', 0);
                    if (separator_pos != std::string::npos)
                    {
                        std::string setting_name =
                            line.substr(0, separator_pos);
                        std::string setting_value =
                            line.substr(separator_pos + 1);

                        settings[current_section][setting_name] = setting_value;
                    }
                }
            }
        }

        return true;
    }

    void configuration_file::save()
    {
        auto configuration_file = file_manager::open_file(
            configuration_path, std::ios_base::out, 100);
        if (configuration_file)
        {
            std::ostream c_stream(configuration_file.get());
            for (auto& section_settings : settings)
            {
                c_stream << '[' << section_settings.first << ']' << std::endl;
                for (auto& setting : section_settings.second)
                {
                    c_stream << setting.first << '=' << setting.second
                             << std::endl;
                }
            }

            c_stream.flush();

            configuration_file->close();
        }
    }

} // namespace Kengine