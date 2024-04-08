#pragma once

#include "Kengine/resources/framebuffer-resource.hxx"

namespace Kengine::graphics
{
    // use Kengine::graphics::rendermanager::push_framebuffer
    class E_DECLSPEC framebuffer
    {
    public:
        framebuffer();
        framebuffer(res_ptr<framebuffer_resource> resource);
        framebuffer(const framebuffer& other);
        ~framebuffer();

        framebuffer& operator=(const framebuffer& other);

        void clear();

        void set_clear_color(const vec4& color);

        [[nodiscard]] inline uint32_t get_id() const
        {
            return resource->get_id();
        }

        [[nodiscard]] inline uint32_t get_color_texture_id() const
        {
            return resource->get_color_texture_id();
        }

        [[nodiscard]] inline uint32_t get_depth_stencil_texture_id() const
        {
            return resource->get_depth_stencil_texture_id();
        }

        [[nodiscard]] inline vec4 get_clear_color() const
        {
            return resource->get_clear_color();
        }

        [[nodiscard]] inline const ivec2& get_size() const
        {
            return resource->get_size();
        }

    private:
        res_ptr<framebuffer_resource> resource;
    };
} // namespace Kengine::graphics