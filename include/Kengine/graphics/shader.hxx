#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "Kengine/graphics/shader-utils/shader-type.hxx"
#include "Kengine/resources/shader-resource.hxx"

namespace Kengine::graphics
{
    class E_DECLSPEC shader
    {
    public:
        shader(const res_ptr<shader_res>& shader_resource);
        shader(const shader& other);
        ~shader();

        shader& operator=(const shader& other);

        void use();

        template <typename T>
        inline void uniform(std::string_view name, T value)
        {
            auto location = get_uniform_location(name);
            set_uniform<T>(location, value);
        };

        void save_uniform_location(std::string_view name);

        void uniform_block(std::string_view block_name, uint32_t end_point);

    private:
        uint32_t get_uniform_location(std::string_view name);

        res_ptr<shader_res> resource;

        std::unordered_map<std::string_view, uint32_t> uniform_locations;
    };
} // namespace Kengine::graphics