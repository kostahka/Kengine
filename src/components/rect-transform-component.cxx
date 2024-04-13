#include "Kengine/components/rect-transform-component.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/components/component-info.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

#include <math.h>

namespace Kengine
{
    rect_transform_component::rect_transform_component()
        : component(name)
        , sc(nullptr){};

    rect_transform_component::rect_transform_component(scene*       sc,
                                                       entt::entity cur_ent)
        : component(name)
        , sc(sc)
        , current_entity(cur_ent){};

    rect_transform_component::rect_transform_component(
        rect_transform_component& other,
        scene*                    sc,
        entt::entity              cur_entity,
        entt::entity              new_parent)
        : component(name)
        , sc(sc)
        , current_entity(cur_entity)
    {
        parent = new_parent;
        transf = other.transf;
    }

    rect_transform_component::rect_transform_component(
        rect_transform_component&& other)
        : component(name)
    {
        std::swap(sc, other.sc);
        std::swap(current_entity, other.current_entity);
        std::swap(parent, other.parent);
        std::swap(transf, other.transf);
    }

    rect_transform_component& rect_transform_component::operator=(
        rect_transform_component& other)
    {
        transf = other.transf;
        return *this;
    }

    rect_transform_component& rect_transform_component::operator=(
        rect_transform_component&& other)
    {
        std::swap(sc, other.sc);
        std::swap(current_entity, other.current_entity);
        std::swap(parent, other.parent);
        std::swap(transf, other.transf);
        return *this;
    }

    std::size_t rect_transform_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, transf.anchor_max.x);
        size += serialization::write(os, transf.anchor_max.y);
        size += serialization::write(os, transf.anchor_min.x);
        size += serialization::write(os, transf.anchor_min.y);
        size += serialization::write(os, transf.delta_min.x);
        size += serialization::write(os, transf.delta_min.y);
        size += serialization::write(os, transf.delta_max.x);
        size += serialization::write(os, transf.delta_max.y);
        size += serialization::write(os, parent);
        size += serialization::write(os, current_entity);

        return size;
    }

    std::size_t rect_transform_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, transf.anchor_max.x);
        size += serialization::read(is, transf.anchor_max.y);
        size += serialization::read(is, transf.anchor_min.x);
        size += serialization::read(is, transf.anchor_min.y);
        size += serialization::read(is, transf.delta_min.x);
        size += serialization::read(is, transf.delta_min.y);
        size += serialization::read(is, transf.delta_max.x);
        size += serialization::read(is, transf.delta_max.y);
        size += serialization::read(is, parent);
        size += serialization::read(is, current_entity);

        return size;
    }

    std::size_t rect_transform_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(transf.anchor_max.x);
        size += serialization::size(transf.anchor_max.y);
        size += serialization::size(transf.anchor_min.x);
        size += serialization::size(transf.anchor_min.y);
        size += serialization::size(transf.delta_min.x);
        size += serialization::size(transf.delta_min.y);
        size += serialization::size(transf.delta_max.x);
        size += serialization::size(transf.delta_max.y);
        size += serialization::size(parent);
        size += serialization::size(current_entity);

        return size;
    }

    bool rect_transform_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);
        edited = edited || ImGui::DragFloat2(
                               "Anchor min", (float*)&transf.anchor_min, 0.1f);
        edited = edited || ImGui::DragFloat2(
                               "Anchor max", (float*)&transf.anchor_max, 0.1f);

        auto world_transform = get_last_transform();
        vec2 world_start     = world_transform.start;
        if (ImGui::DragFloat2("Start", (float*)&world_start, 1.f))
        {
            transf.delta_min += world_start - world_transform.start;
            edited = true;
            invalidate_transform();
        }
        vec2 world_rect = world_transform.rect;
        if (ImGui::DragFloat2("Rect", (float*)&world_rect, 1.f))
        {
            transf.delta_max += world_rect - world_transform.rect;
            edited = true;
            invalidate_transform();
        }

        ImGui::PopID();
#endif
        return edited;
    }

    rect_transform rect_transform_component::get_world_transform()
    {
        if (!sc->registry.valid(parent))
        {
            parent = entt::null;
        }

        rect_transform parent_transform;

        if (!sc || parent == entt::null)
        {
            auto current_viewport   = graphics::get_current_viewport();
            vec2 f_current_viewport = { (float)current_viewport.x,
                                        (float)current_viewport.y };
            parent_transform.start  = { 0, 0 };
            parent_transform.rect   = f_current_viewport;
        }
        else
        {
            parent_transform =
                sc->registry.get<rect_transform_component>(parent)
                    .get_world_transform();
        }

        rect_transform result;
        result.start = transf.anchor_min * parent_transform.rect +
                       parent_transform.start + transf.delta_min;
        result.rect = (transf.anchor_max * parent_transform.rect +
                       parent_transform.start + transf.delta_max) -
                      result.start;

        if (result.rect.x < 0)
            result.rect.x = 0;
        if (result.rect.y < 0)
            result.rect.y = 0;

        is_trans_valid = true;
        last_trans     = result;

        return result;
    }

    void rect_transform_component::set_parent(entt::entity ent)
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

    bool rect_transform_component::check_for_parent(entt::entity ent)
    {
        bool result = current_entity != ent;

        if (sc->registry.valid(parent))
        {
            return result && sc->registry.get<rect_transform_component>(parent)
                                 .check_for_parent(ent);
        }

        return result;
    }

    template <>
    void archive_input::operator()(rect_transform_component& value)
    {
        total_size += serialization::read(is, value);
        value.initialize(&sc);
    }

    component_info rect_transform_component::info{
        [](scene& sc, entt::entity ent)
        { return sc.registry.any_of<rect_transform_component>(ent); },

        [](scene& sc, entt::entity ent)
        {
            return static_cast<component*>(
                &sc.registry.get<rect_transform_component>(ent));
        },

        [](entt::snapshot& snapshot, archive_output& output)
        { snapshot.get<rect_transform_component>(output); },

        [](entt::snapshot& snapshot, archive_size& output)
        { snapshot.get<rect_transform_component>(output); },

        [](entt::snapshot_loader& snapshot, archive_input& input)
        { snapshot.get<rect_transform_component>(input); },

        [](entt::continuous_loader& snapshot, archive_input& input)
        { snapshot.get<rect_transform_component>(input); },

        [](scene& sc, entt::entity ent)
        { sc.registry.erase<rect_transform_component>(ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.emplace<rect_transform_component>(ent, &sc, ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.patch<rect_transform_component>(ent); },

        [](scene&                                         sc,
           entt::entity                                   ent,
           component*                                     other,
           std::unordered_map<entt::entity, entt::entity> map)
        {
            auto   other_transform =
                static_cast<rect_transform_component*>(other);
            auto   new_parent_it = map.find(other_transform->parent);
            auto   new_parent =
                new_parent_it != map.end() ? new_parent_it->second : entt::null;
            sc.registry.emplace<rect_transform_component>(
                ent, *other_transform, &sc, ent, new_parent);
        },
    };
} // namespace Kengine