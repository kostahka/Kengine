#pragma once

#include "Kengine/units/vector.hxx"
#include <string>

namespace Kengine
{

    class texture_object
    {
    public:
        virtual ~texture_object() = default;

        virtual void                   bind()           = 0;
        [[nodiscard]] virtual ivec2    get_size() const = 0;
        [[nodiscard]] virtual intptr_t get_id() const   = 0;
    };

    texture_object *create_texture(std::string texture_path);

} // namespace Kengine
