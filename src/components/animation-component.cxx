#include "Kengine/components/animation-component.hxx"

#include "Kengine/components/component-info.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

namespace Kengine
{
    animation_component::animation_component()
        : component(name)
    {
        is_playing = is_play_on_start;
    }

    animation_component::animation_component(
        const res_ptr<animation_resource>& res)
        : component(name)
        , anim_res(res)
    {
        if (anim_res)
        {
            anim_res->take_data();
        }
        is_playing = is_play_on_start;
    }

    animation_component::animation_component(animation_component& other)
        : component(name)
    {
        is_play_on_start  = other.is_play_on_start;
        is_looped         = other.is_looped;
        delta_frame_time  = other.delta_frame_time;
        current_animation = other.current_animation;
        anim_res          = other.anim_res;
        if (anim_res)
        {
            anim_res->take_data();
        }
        is_playing = is_play_on_start;
    }

    animation_component::animation_component(animation_component&& other)
        : component(name)
    {
        is_play_on_start  = other.is_play_on_start;
        is_looped         = other.is_looped;
        delta_frame_time  = other.delta_frame_time;
        current_animation = other.current_animation;
        std::swap(anim_res, other.anim_res);
        is_playing = is_play_on_start;
    }

    animation_component& animation_component::operator=(
        animation_component& other)
    {
        if (anim_res)
        {
            anim_res->free_data();
        }
        is_play_on_start  = other.is_play_on_start;
        is_looped         = other.is_looped;
        delta_frame_time  = other.delta_frame_time;
        current_animation = other.current_animation;
        anim_res          = other.anim_res;
        if (anim_res)
        {
            anim_res->take_data();
        }
        is_playing = is_play_on_start;
        return *this;
    }

    animation_component& animation_component::operator=(
        animation_component&& other)
    {
        if (anim_res)
        {
            anim_res->free_data();
        }
        is_play_on_start  = other.is_play_on_start;
        is_looped         = other.is_looped;
        delta_frame_time  = other.delta_frame_time;
        current_animation = other.current_animation;
        anim_res          = other.anim_res;
        std::swap(anim_res, other.anim_res);
        is_playing = is_play_on_start;
        return *this;
    }

    animation_component::~animation_component()
    {
        if (anim_res)
        {
            anim_res->free_data();
        }
    }

    void animation_component::set_current_frame(int frame)
    {
        if (anim_res)
        {
            current_frame = frame;
            current_frame = current_frame % anim_res->get_animations()
                                                .find(current_animation)
                                                ->second.size();
        }
    }

    void animation_component::set_current_animation(string_id id)
    {
        if (anim_res)
        {
            if (anim_res->get_animations().find(current_animation) !=
                anim_res->get_animations().end())
            {
                current_animation = id;
            }
        }
    }

    void animation_component::resume()
    {
        is_playing = true;
    }

    void animation_component::restart()
    {
        is_playing    = true;
        current_frame = 0;
    }

    void animation_component::pause()
    {
        is_playing = false;
    }

    void animation_component::stop()
    {
        is_playing    = false;
        current_frame = 0;
    }

    std::size_t animation_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, is_play_on_start);
        size += serialization::write(os, delta_frame_time);
        size += serialization::write(os, current_animation);
        size += serialization::write(os, anim_res);
        size += serialization::write(os, reversed);
        size += serialization::write(os, is_looped);

        return size;
    }

    std::size_t animation_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, is_play_on_start);
        size += serialization::read(is, delta_frame_time);
        size += serialization::read(is, current_animation);
        size += serialization::read(is, anim_res);
        size += serialization::read(is, reversed);
        size += serialization::read(is, is_looped);
        is_playing = is_play_on_start;

        return size;
    }

    std::size_t animation_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(is_play_on_start);
        size += serialization::size(delta_frame_time);
        size += serialization::size(current_animation);
        size += serialization::size(anim_res);
        size += serialization::size(reversed);
        size += serialization::size(is_looped);

        return size;
    }

    bool animation_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);
        edited |= ImGui::Checkbox("Is play on start", &is_play_on_start);
        edited |= ImGui::Checkbox("Is looped", &is_looped);
        edited |= ImGui::Checkbox("Reversed", &reversed);
        edited |= ImGui::DragInt("Delta frame time", &delta_frame_time);

        if (anim_res)
        {
            auto anim_it = anim_res->get_animations().find(current_animation);
            if (ImGui::DragInt("Current frame",
                               &current_frame,
                               1,
                               0,
                               static_cast<int>(anim_it->second.size())))
            {
                edited = true;
            }
            for (auto anim_it : anim_res->get_animations())
            {
                ImGui::PushID(anim_it.first.get_id());
                ImGui::Text("%s", anim_it.first.get_string());
                ImGui::SameLine();
                if (ImGui::Button("Select"))
                {
                    edited            = true;
                    current_animation = anim_it.first;
                }
                ImGui::PopID();
            }
        }

        auto new_anim_res = anim_res;
        if (imgui::edit_resource("Animation resource", &new_anim_res))
        {
            if (anim_res)
            {
                anim_res->free_data();
            }
            anim_res = new_anim_res;
            if (anim_res)
            {
                anim_res->take_data();
            }
            edited = true;
        }

        if (ImGui::Button("Resume"))
        {
            resume();
        }
        if (ImGui::Button("Restart"))
        {
            restart();
        }
        if (ImGui::Button("Pause"))
        {
            pause();
        }
        if (ImGui::Button("Stop"))
        {
            stop();
        }

        ImGui::PopID();
#endif
        return edited;
    }

} // namespace Kengine