#pragma once

#include "Kengine/graphics/shader-utils/shader-type.hxx"

#include <cstdint>
#include <type_traits>

namespace Kengine::graphics
{
    class uniformbuffer
    {
    public:
        uniformbuffer();
        ~uniformbuffer();

        void bind();
        void uniform_data(const void* data,
                          uint32_t    size,
                          bool        dynamic); // bind before use
        void uniform_subdata(const void* data,
                             uint32_t    size,
                             uint32_t    offset); // bind before use

        void bind_end_point(uint32_t end_point);

    private:
        uint32_t id;
    };

    template <typename... Types>
    class uniformbuffer_std140 : public uniformbuffer

    {
    public:
        uniformbuffer_std140(bool dynamic, const Types&... args)
            : uniformbuffer()
        {
            std::pair<uint32_t, char*> data_info =
                std140::get_buffer(aligns, args...);

            bind();
            uniform_data(data_info.second, data_info.first, dynamic);

            total_size = data_info.first;
            data       = data_info.second;
        };

        template <int I>
        void set_value(const std140::type_at<I, Types...>& value)
        {
            using value_type = std140::type_at<I, Types...>;

            uniform_subdata(
                get_shader_type_address(value), sizeof(value_type), aligns[I]);
        }

        void set_values(const Types&... args)
        {
            std140::fill_data(data, aligns, args...);
            uniform_subdata(data, total_size, 0);
        }

    private:
        uint32_t aligns[sizeof...(Types)];
        uint32_t total_size;
        char*    data;
    };
} // namespace Kengine::graphics