#pragma once

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/types/vector.hxx"
#include "resource.hxx"

namespace Kengine
{
    class E_DECLSPEC texture_resource : public resource
    {
    public:
        texture_resource(std::string_view name);
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
        std::size_t serialize_size() const override;

        [[nodiscard]] inline uint32_t get_id() const
        {

            ;
            ;

            return id;
        }

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

        bool imgui_editable_render() override;

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

        void load_invalid_texture();

        ivec2    size         = { 1, 1 };
        int      num_channels = 0;
        uint32_t id           = 0;

        texture_filter mag_filter = texture_filter::linear;
        texture_filter min_filter = texture_filter::linear;

        texture_format format = texture_format::none;
    };
} // namespace Kengine