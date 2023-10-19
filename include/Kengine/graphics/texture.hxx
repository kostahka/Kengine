#pragma once

#include "Kengine/resources/texture-resource.hxx"

#include <memory>

namespace Kengine::graphics
{
    class texture
    {
    public:
        texture(std::shared_ptr<texture_resource> resource);
        ~texture();

        void bind();

        [[nodiscard]] inline uint32_t get_id() const
        {
            return resource->get_id();
        }

        [[nodiscard]] inline ivec2 get_size() const
        {
            return resource->get_size();
        }

    private:
        std::shared_ptr<texture_resource> resource;
    };
} // namespace Kengine::graphics