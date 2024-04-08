#include "Kengine/components/transform-component.hxx"

#include "Kengine/components/component-info.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

#include <math.h>

namespace Kengine
{
    transform_component::transform_component()
        : component(name)
        , sc(nullptr){};

    transform_component::transform_component(scene* sc, entt::entity cur_ent)
        : component(name)
        , sc(sc)
        , current_entity(cur_ent){};

    transform_component::transform_component(transform_component& other,
                                             scene*               sc,
                                             entt::entity         cur_entity,
                                             entt::entity         new_parent)
        : component(name)
        , sc(sc)
        , current_entity(cur_entity)
    {
        parent = new_parent;
        transf = other.transf;
    }

    transform_component::transform_component(transform_component&& other)
        : component(name)
    {
        std::swap(sc, other.sc);
        std::swap(current_entity, other.current_entity);
        std::swap(parent, other.parent);
        std::swap(transf, other.transf);
    }

    transform_component& transform_component::operator=(
        transform_component& other)
    {
        transf = other.transf;
        return *this;
    }

    transform_component& transform_component::operator=(
        transform_component&& other)
    {
        std::swap(sc, other.sc);
        std::swap(current_entity, other.current_entity);
        std::swap(parent, other.parent);
        std::swap(transf, other.transf);
        return *this;
    }

    std::size_t transform_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, transf.position.x);
        size += serialization::write(os, transf.position.y);
        size += serialization::write(os, transf.angle);
        size += serialization::write(os, transf.scale.x);
        size += serialization::write(os, transf.scale.y);
        size += serialization::write(os, transf.scale.z);
        size += serialization::write(os, parent);
        size += serialization::write(os, current_entity);

        return size;
    }

    std::size_t transform_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, transf.position.x);
        size += serialization::read(is, transf.position.y);
        size += serialization::read(is, transf.angle);
        size += serialization::read(is, transf.scale.x);
        size += serialization::read(is, transf.scale.y);
        size += serialization::read(is, transf.scale.z);
        size += serialization::read(is, parent);
        size += serialization::read(is, current_entity);

        return size;
    }

    std::size_t transform_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(transf.position.x);
        size += serialization::size(transf.position.y);
        size += serialization::size(transf.angle);
        size += serialization::size(transf.scale.x);
        size += serialization::size(transf.scale.y);
        size += serialization::size(transf.scale.z);
        size += serialization::size(parent);
        size += serialization::size(current_entity);

        return size;
    }

    bool transform_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);
        edited = edited ||
                 ImGui::DragFloat2("Position", (float*)&transf.position, 0.1f);
        edited = edited || ImGui::DragFloat("Angle", &transf.angle, 0.1f);
        edited =
            edited || ImGui::DragFloat3("Scale", (float*)&transf.scale, 0.1f);
        ImGui::PopID();
#endif
        return edited;
    }

    transform transform_component::get_world_transform()
    {
        if (!sc || parent == entt::null)
        {
            return transf;
        }

        if (!sc->registry.valid(parent))
        {
            parent = entt::null;
            return transf;
        }

        Kengine::transform result =
            sc->registry.get<transform_component>(parent).get_world_transform();

        float res_sin = static_cast<float>(sin(result.angle));
        float res_cos = static_cast<float>(cos(result.angle));

        result.scale.x = transf.scale.x;
        result.scale.y = transf.scale.y;

        result.position.x +=
            transf.position.x * res_cos - transf.position.y * res_sin;
        result.position.y +=
            transf.position.x * res_sin + transf.position.y * res_cos;

        result.angle += transf.angle;

        return result;
    }

    void transform_component::set_parent(entt::entity ent)
    {
        if (ent != entt::null)
        {
            if (!sc->registry.valid(ent) || !check_for_parent(ent))
            {
                return;
            }
        }
        parent = ent;
    }

    bool transform_component::check_for_parent(entt::entity ent)
    {
        bool result = current_entity != ent;

        if (sc->registry.valid(parent))
        {
            return result && sc->registry.get<transform_component>(parent)
                                 .check_for_parent(ent);
        }

        return result;
    }

    template <>
    void archive_input::operator()(transform_component& value)
    {
        total_size += serialization::read(is, value);
        value.initialize(&sc);
    }

    component_info transform_component::info{
        [](scene& sc, entt::entity ent)
        { return sc.registry.any_of<transform_component>(ent); },

        [](scene& sc, entt::entity ent) {
            return static_cast<component*>(
                &sc.registry.get<transform_component>(ent));
        },

        [](entt::snapshot& snapshot, archive_output& output)
        { snapshot.get<transform_component>(output); },

        [](entt::snapshot& snapshot, archive_size& output)
        { snapshot.get<transform_component>(output); },

        [](entt::snapshot_loader& snapshot, archive_input& input)
        { snapshot.get<transform_component>(input); },

        [](entt::continuous_loader& snapshot, archive_input& input)
        { snapshot.get<transform_component>(input); },

        [](scene& sc, entt::entity ent)
        { sc.registry.erase<transform_component>(ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.emplace<transform_component>(ent, &sc, ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.patch<transform_component>(ent); },

        [](scene&                                         sc,
           entt::entity                                   ent,
           component*                                     other,
           std::unordered_map<entt::entity, entt::entity> map)
        {
            auto   other_transform = static_cast<transform_component*>(other);
            auto   new_parent_it   = map.find(other_transform->parent);
            auto   new_parent =
                new_parent_it != map.end() ? new_parent_it->second : entt::null;
            sc.registry.emplace<transform_component>(
                ent, *other_transform, &sc, ent, new_parent);
        },
    };
} // namespace Kengine