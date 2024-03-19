#pragma once

#include "Kengine/resources/resource-manager.hxx"
#include "renderbuffer-resource.hxx"
#include "resource.hxx"
#include "texture-resource.hxx"

namespace Kengine
{
    class framebuffer_resource : public resource
    {
    public:
        framebuffer_resource(std::string_view name);
        /// attachments can be texture_resource or renderbuffer_resource
        framebuffer_resource(res_ptr<resource> color_attachment,
                             res_ptr<resource> depth_stencil_attachment,
                             const vec4&       clear_color,
                             std::string_view  name);

        ~framebuffer_resource() override;

        void set_clear_color(const vec4& clear_color);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline uint32_t get_color_texture_id() const
        {
            return color_id;
        }

        [[nodiscard]] inline uint32_t get_depth_stencil_texture_id() const
        {
            return depth_stencil_id;
        }

        [[nodiscard]] inline const vec4& get_clear_color() const
        {
            return clear_color;
        }

        [[nodiscard]] inline bool is_color_attach() const
        {
            return color_attach;
        }

        [[nodiscard]] inline bool is_depth_attach() const
        {
            return depth_attach;
        }

        [[nodiscard]] inline bool is_stencil_attach() const
        {
            return stencil_attach;
        }

        [[nodiscard]] inline const ivec2& get_size() const { return size; }

        bool imgui_editable_render() override;

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        void attachment(res_ptr<resource>, bool color_attachment);

        res_ptr<resource> color_attachment;
        res_ptr<resource> depth_stencil_attachment;
        uint32_t          id;
        uint32_t          color_id;
        uint32_t          depth_stencil_id;

        vec4  clear_color;
        ivec2 size;

        bool color_attach;
        bool depth_attach;
        bool stencil_attach;
    };
} // namespace Kengine