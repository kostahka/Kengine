#pragma once

#include "Kengine/serialization/serialization.hxx"
#include "entt/entt.hpp"

namespace Kengine
{
    class archive_input
    {
    public:
        archive_input(std::istream& is);

        void operator()(entt::entity&);
        void operator()(std::underlying_type_t<entt::entity>&);

        template <typename T>
        void operator()(T& value)
        {
            serialization::read(is, value);
        }

        inline std::size_t get_size() const { return total_size; }

    private:
        std::istream& is;
        std::size_t   total_size;
    };

    class archive_output
    {
    public:
        archive_output(std::ostream& os);

        void operator()(entt::entity);
        void operator()(std::underlying_type_t<entt::entity>);

        template <typename T>
        void operator()(const T& value)
        {
            serialization::write(os, value);
        }

        inline std::size_t get_size() const { return total_size; }

    private:
        std::ostream& os;
        std::size_t   total_size;
    };
} // namespace Kengine