#pragma once

#include "Kengine/serialization/archive.hxx"
#include "entt/entt.hpp"

namespace Kengine
{
    typedef void (*serialize_component_fp)(entt::snapshot& snapshot,
                                           archive_output& output);
    typedef void (*deserialize_component_fp)(entt::snapshot_loader& snapshot,
                                             archive_input&         input);

    struct component_info
    {
        serialize_component_fp   serialize_component   = nullptr;
        deserialize_component_fp deserialize_component = nullptr;
    };
} // namespace Kengine