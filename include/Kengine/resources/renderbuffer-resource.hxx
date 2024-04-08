#pragma once

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/units/vector.hxx"
#include "resource.hxx"

namespace Kengine
{
    class E_DECLSPEC renderbuffer_resource : public resource
    {
    public:
        renderbuffer_resource(std::string_view name);
        renderbuffer_resource(ivec2                   size,
                              texture_internal_format format,
                              std::string_view        name);
        ~renderbuffer_resource() override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline const ivec2& get_size() const { return size; }

        [[nodiscard]] inline texture_internal_format get_format() const
        {
            return format;
        }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        ivec2                   size;
        texture_internal_format format;
        uint32_t                id;
    };
} // namespace Kengine