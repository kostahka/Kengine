#pragma once

#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/scene/scene.hxx"
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
            total_size += serialization::read(is, value);
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
            total_size += serialization::write(os, value);
        }

        inline std::size_t get_size() const { return total_size; }

    private:
        std::ostream& os;
        const scene&  sc;
        std::size_t   total_size;
    };

    class archive_size
    {
    public:
        archive_size(const scene& sc);

        void operator()(entt::entity);
        void operator()(std::underlying_type_t<entt::entity>);

        template <typename T>
        void operator()(const T& value)
        {
            total_size += serialization::size(value);
        }

        inline std::size_t get_size() const { return total_size; }

    private:
        const scene& sc;
        std::size_t  total_size;
    };
} // namespace Kengine