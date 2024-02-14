#pragma once

#include "Kengine/scene/scene.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "Kengine/string/string-id.hxx"

#include <filesystem>
#include <memory>

namespace Kengine
{
    class scene_link : public serializable
    {
    public:
        scene_link();
        scene_link(std::string_view name, std::filesystem::path sc_path);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        inline const std::filesystem::path& get_path() const { return sc_path; }

        inline string_id get_name_id() const { return name_id; }

    private:
        string_id             name_id;
        std::filesystem::path sc_path;
    };
} // namespace Kengine