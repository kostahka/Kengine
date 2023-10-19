#pragma once

#include "Kengine/units/vector.hxx"
#include "resource.hxx"

namespace Kengine
{
    class texture_resource : public resource
    {
    public:
        texture_resource(path texture_path);
        ~texture_resource() override;

        [[nodiscard]] inline uint32_t get_id() const { return id; }

        [[nodiscard]] inline ivec2 get_size() const { return size; }

    protected:
        virtual void load_data() override;
        virtual void unload_data() override;

    private:
        ivec2    size;
        uint32_t id;
    };
} // namespace Kengine