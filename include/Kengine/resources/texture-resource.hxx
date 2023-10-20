#pragma once

#include "Kengine/units/vector.hxx"
#include "resource.hxx"

namespace Kengine
{
    enum class texture_filter
    {
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    class texture_resource : public resource
    {
    public:
        texture_resource(path           texture_path,
                         texture_filter mag_filter = texture_filter::linear,
                         texture_filter min_filter = texture_filter::linear);
        ~texture_resource() override;

        void set_filter(texture_filter min_filter, texture_filter mag_filter);

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline ivec2 get_size() const { return size; }

        [[nodiscard]] inline texture_filter get_min_filter() const
        {
            return min_filter;
        }

        [[nodiscard]] inline texture_filter get_mag_filter() const
        {
            return mag_filter;
        }

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        void load_invalid_texture();

        ivec2    size;
        int      num_channels;
        uint32_t id;

        texture_filter mag_filter;
        texture_filter min_filter;
    };
} // namespace Kengine