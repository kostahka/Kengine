#pragma once

#include "Kengine/configuration/configuration.hxx"

namespace Kengine
{
    class E_DECLSPEC configuration_file : public configuration
    {
    public:
        configuration_file(std::string_view name);
        ~configuration_file();

        bool load();

        void save();

    private:
        std::string           name;
        std::filesystem::path configuration_path;
    };
} // namespace Kengine