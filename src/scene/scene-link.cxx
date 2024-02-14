#include "Kengine/scene/scene-link.hxx"

#include <string>

namespace Kengine
{
    scene_link::scene_link()
        : name_id(0)
        , sc_path("")
    {
    }

    scene_link::scene_link(std::string_view name, std::filesystem::path sc_path)
        : sc_path(sc_path)
    {
        name_id = hash_string(name.data());
    }

    std::size_t scene_link::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        auto name        = std::string(get_string(name_id));
        auto sc_path_str = sc_path.string();

        size += serialization::write(os, name);
        size += serialization::write(os, sc_path_str);

        return size;
    }

    std::size_t scene_link::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        std::string name;
        std::string sc_path_str;

        size += serialization::read(is, name);
        size += serialization::read(is, sc_path_str);

        name_id = hash_string(name.c_str());
        sc_path = std::filesystem::path(sc_path_str);

        return size;
    }
} // namespace Kengine