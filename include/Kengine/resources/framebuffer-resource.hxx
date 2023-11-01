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
        /// attachments can be texture_resource or renderbuffer_resource
        framebuffer_resource(res_ptr<resource> color_attachment,
                             res_ptr<resource> depth_stencil_attachment,
                             std::string_view  name);

        ~framebuffer_resource() override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline uint32_t& get_color_texture_id()
        {
            return color_id;
        }

        [[nodiscard]] inline uint32_t& get_depth_stencil_texture_id()
        {
            return depth_stencil_id;
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

        bool color_attach;
        bool depth_attach;
        bool stencil_attach;
    };
} // namespace Kengine