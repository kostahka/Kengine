#pragma once

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/units/vector.hxx"
#include "resource.hxx"

namespace Kengine
{
    class texture_resource : public resource
    {
    public:
        texture_resource(path             texture_path,
                         std::string_view name,
                         texture_filter   mag_filter = texture_filter::linear,
                         texture_filter   min_filter = texture_filter::linear);
        texture_resource(ivec2            size,
                         texture_format   format,
                         std::string_view name,
                         texture_filter   mag_filter = texture_filter::linear,
                         texture_filter   min_filter = texture_filter::linear);
        ~texture_resource() override;

        void set_filter(texture_filter min_filter, texture_filter mag_filter);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline const ivec2& get_size() const { return size; }

        [[nodiscard]] inline texture_filter get_min_filter() const
        {
            return min_filter;
        }

        [[nodiscard]] inline texture_filter get_mag_filter() const
        {
            return mag_filter;
        }

        [[nodiscard]] inline texture_format get_format() const
        {
            return format;
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

        texture_format format;
    };
} // namespace Kengine