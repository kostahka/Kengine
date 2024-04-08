#pragma once

#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/resources/texture-resource.hxx"

namespace Kengine::graphics
{
    class E_DECLSPEC texture
    {
    public:
        texture(res_ptr<texture_resource> resource);
        texture(const texture& other);
        ~texture();

        texture& operator=(const texture& other);

        void bind(uint32_t texture_block = 0);

        [[nodiscard]] inline uint32_t get_id() const
        {
            return resource->get_id();
        }

        [[nodiscard]] inline const ivec2& get_size() const
        {
            return resource->get_size();
        }

    private:
        res_ptr<texture_resource> resource;
    };
} // namespace Kengine::graphics