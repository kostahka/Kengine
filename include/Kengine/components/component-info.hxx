#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/scene/scene.hxx"
#include "Kengine/serialization/archive.hxx"

#include "entt/entt.hpp"

namespace Kengine
{
    typedef bool (*has_component_fp)(scene&, entt::entity);
    typedef component* (*get_component_fp)(scene&, entt::entity);
    typedef void (*serialize_component_fp)(entt::snapshot&, archive_output&);
    typedef void (*deserialize_component_fp)(entt::snapshot_loader&,
                                             archive_input&);

    typedef void (*erase_component_fp)(scene&, entt::entity);
    typedef void (*emplace_component_fp)(scene&, entt::entity);
    typedef void (*patch_component_fp)(scene&, entt::entity);

    struct component_info
    {
        has_component_fp         has_component         = nullptr;
        get_component_fp         get_component         = nullptr;
        serialize_component_fp   serialize_component   = nullptr;
        deserialize_component_fp deserialize_component = nullptr;
        erase_component_fp       erase_component       = nullptr;
        emplace_component_fp     emplace_component     = nullptr;
        patch_component_fp       patch_component       = nullptr;
    };

    template <typename ComponentType>
    component_info create_component_info()
    {
        static_assert(std::is_base_of_v<component, ComponentType>,
                      "ComponentType only inhertis component");

        component_info c_info;

        c_info.has_component = [](scene& sc, entt::entity ent)
        { return sc.registry.any_of<ComponentType>(ent); };

        c_info.get_component = [](scene& sc, entt::entity ent) {
            return static_cast<component*>(
                &sc.registry.get<ComponentType>(ent));
        };

        c_info.serialize_component =
            [](entt::snapshot& snapshot, archive_output& output)
        { snapshot.get<ComponentType>(output); };

        c_info.deserialize_component =
            [](entt::snapshot_loader& snapshot, archive_input& input)
        { snapshot.get<ComponentType>(input); };

        c_info.erase_component = [](scene& sc, entt::entity ent)
        { sc.registry.erase<ComponentType>(ent); };

        c_info.emplace_component = [](scene& sc, entt::entity ent)
        { sc.registry.emplace<ComponentType>(ent); };

        c_info.patch_component = [](scene& sc, entt::entity ent)
        { sc.registry.patch<ComponentType>(ent); };

        return c_info;
    }
} // namespace Kengine