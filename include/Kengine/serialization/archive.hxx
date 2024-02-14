#pragma once

#include "Kengine/components/camera-component.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "entt/entt.hpp"

namespace Kengine
{
    class archive_input
    {
    public:
        archive_input(std::istream& is, scene& sc);

        void operator()(entt::entity&);
        void operator()(std::underlying_type_t<entt::entity>&);

        template <typename T>
        void operator()(T& value)
        {
            serialization::read(is, value);
        }

        void operator()(camera_component& value)
        {
            serialization::read(is, value);
            if (value.is_binded())
            {
                value.bind(sc);
            }
        }

        inline std::size_t get_size() const { return total_size; }

    private:
        std::istream& is;
        scene&        sc;
        std::size_t   total_size;
    };

    class archive_output
    {
    public:
        archive_output(std::ostream& os, const scene& sc);

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
        const scene&  sc;
        std::size_t   total_size;
    };
} // namespace Kengine