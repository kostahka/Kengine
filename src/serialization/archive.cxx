#include "Kengine/serialization/archive.hxx"

namespace Kengine
{
    archive_input::archive_input(std::istream& is, scene& sc)
        : is(is)
        , sc(sc)
        , total_size(0)
    {
    }

    void archive_input::operator()(entt::entity& ent)
    {
        total_size += serialization::read(is, ent);
    }

    void archive_input::operator()(std::underlying_type_t<entt::entity>& size)
    {
        total_size += serialization::read(is, size);
    }

    archive_output::archive_output(std::ostream& os, const scene& sc)
        : os(os)
        , sc(sc)
        , total_size(0)
    {
    }

    void archive_output::operator()(entt::entity ent)
    {
        total_size += serialization::write(os, ent);
    }

    void archive_output::operator()(std::underlying_type_t<entt::entity> size)
    {
        total_size += serialization::write(os, size);
    }

    archive_size::archive_size(const scene& sc)
        : sc(sc)
        , total_size(0)
    {
    }

    void archive_size::operator()(entt::entity ent)
    {
        total_size += serialization::size(ent);
    }

    void archive_size::operator()(std::underlying_type_t<entt::entity> size)
    {
        total_size += serialization::size(size);
    }
} // namespace Kengine