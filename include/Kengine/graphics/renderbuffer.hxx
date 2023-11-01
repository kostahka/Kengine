#pragma once

#include "Kengine/resources/renderbuffer-resource.hxx"
#include "Kengine/resources/res-ptr.hxx"

namespace Kengine::graphics
{
    class renderbuffer
    {
    public:
        renderbuffer(res_ptr<renderbuffer_resource> resource);
        renderbuffer(const renderbuffer& other);
        ~renderbuffer();

        renderbuffer& operator=(const renderbuffer& other);

        [[nodiscard]] inline uint32_t get_id() const
        {
            return resource->get_id();
        }

        void bind();

    private:
        res_ptr<renderbuffer_resource> resource;
    };
} // namespace Kengine::graphics