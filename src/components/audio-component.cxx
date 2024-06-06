#include "Kengine/components/audio-component.hxx"

#include "Kengine/components/component-info.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/scene/scene.hxx"

#include "imgui.h"

namespace Kengine
{
    audio_component::audio_component()
        : component(name)
    {
    }

    audio_component::audio_component(const res_ptr<audio_resource>& res)
        : component(name)
    {
        continuous_object = audio::create_continuous_object(res);
    }

    audio_component::audio_component(audio_component& other)
        : component(name)
    {
        is_play_on_start   = other.is_play_on_start;
        is_surrounded      = other.is_surrounded;
        max_distance       = other.max_distance;
        channels_intensity = other.channels_intensity;
        volume             = other.volume;
        if (other.continuous_object)
        {
            continuous_object = audio::create_continuous_object(
                other.continuous_object->get_res());
            continuous_object->is_looped = other.continuous_object->is_looped;
            continuous_object->set_volume(
                other.continuous_object->get_volume());
        }
    }

    audio_component::audio_component(audio_component&& other)
        : component(name)
    {
        is_play_on_start   = other.is_play_on_start;
        is_surrounded      = other.is_surrounded;
        max_distance       = other.max_distance;
        channels_intensity = other.channels_intensity;
        volume             = other.volume;
        std::swap(continuous_object, other.continuous_object);
    }

    audio_component& audio_component::operator=(audio_component& other)
    {
        if (continuous_object)
        {
            audio::destroy_continuous_object(continuous_object);
        }
        is_play_on_start   = other.is_play_on_start;
        is_surrounded      = other.is_surrounded;
        max_distance       = other.max_distance;
        channels_intensity = other.channels_intensity;
        volume             = other.volume;
        if (other.continuous_object)
        {
            continuous_object = audio::create_continuous_object(
                other.continuous_object->get_res());
            continuous_object->is_looped = other.continuous_object->is_looped;
            continuous_object->set_volume(
                other.continuous_object->get_volume());
        }
        return *this;
    }

    audio_component& audio_component::operator=(audio_component&& other)
    {
        if (continuous_object)
        {
            audio::destroy_continuous_object(continuous_object);
        }
        is_play_on_start   = other.is_play_on_start;
        is_surrounded      = other.is_surrounded;
        max_distance       = other.max_distance;
        channels_intensity = other.channels_intensity;
        volume             = other.volume;
        std::swap(continuous_object, other.continuous_object);
        return *this;
    }

    audio_component::~audio_component()
    {
        if (continuous_object)
        {
            audio::destroy_continuous_object(continuous_object);
        }
    }

    void audio_component::set_volume(float volume)
    {
        if (volume > 1)
        {
            volume = 1;
        }
        else if (volume < 0)
        {
            volume = 0;
        }
        this->volume = volume;
        if (continuous_object)
        {
            continuous_object->set_volume(this->volume *
                                          this->channels_intensity);
        }
    }

    void audio_component::set_channels_intensity(vec2 channels_intensity)
    {
        if (channels_intensity.x > 1)
        {
            channels_intensity.x = 1;
        }
        else if (channels_intensity.x < 0)
        {
            channels_intensity.x = 0;
        }
        if (channels_intensity.y > 1)
        {
            channels_intensity.y = 1;
        }
        else if (channels_intensity.y < 0)
        {
            channels_intensity.y = 0;
        }
        this->channels_intensity = channels_intensity;
        if (continuous_object)
        {
            continuous_object->set_volume(this->volume *
                                          this->channels_intensity);
        }
    }

    void audio_component::resume()
    {
        if (continuous_object)
        {
            continuous_object->resume();
        }
    }

    void audio_component::restart()
    {
        if (continuous_object)
        {
            continuous_object->restart();
        }
    }

    void audio_component::pause()
    {
        if (continuous_object)
        {
            continuous_object->pause();
        }
    }

    void audio_component::stop()
    {
        if (continuous_object)
        {
            continuous_object->stop();
        }
    }

    void audio_component::play_one_shot()
    {
        if (continuous_object)
        {
            audio::play_one_shot(continuous_object->get_res(),
                                 volume * channels_intensity);
        }
    }

    std::size_t audio_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, is_play_on_start);
        size += serialization::write(os, is_surrounded);
        size += serialization::write(os, max_distance);
        size += serialization::write(os, channels_intensity.x);
        size += serialization::write(os, channels_intensity.y);
        size += serialization::write(os, volume);
        if (continuous_object)
        {
            size += serialization::write(os, continuous_object->get_res());
            size += serialization::write(os, continuous_object->is_looped);
        }
        else
        {
            res_ptr<audio_resource> res = nullptr;
            size += serialization::write(os, res);
        }

        return size;
    }

    std::size_t audio_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        size += serialization::read(is, is_play_on_start);
        size += serialization::read(is, is_surrounded);
        size += serialization::read(is, max_distance);
        size += serialization::read(is, channels_intensity.x);
        size += serialization::read(is, channels_intensity.y);
        size += serialization::read(is, volume);

        res_ptr<audio_resource> res = nullptr;
        size += serialization::read(is, res);
        continuous_object = audio::create_continuous_object(res);
        if (continuous_object)
        {
            size += serialization::read(is, continuous_object->is_looped);
            continuous_object->set_volume(volume * channels_intensity);
        }

        return size;
    }

    std::size_t audio_component::serialize_size() const
    {
        std::size_t size = 0;

        size += serialization::size(is_play_on_start);
        size += serialization::size(is_surrounded);
        size += serialization::size(max_distance);
        size += serialization::size(channels_intensity.x);
        size += serialization::size(channels_intensity.y);
        size += serialization::size(volume);
        if (continuous_object)
        {
            size += serialization::size(continuous_object->get_res());
            size += serialization::size(continuous_object->is_looped);
        }
        else
        {
            res_ptr<audio_resource> res = nullptr;
            size += serialization::size(res);
        }

        return size;
    }

    bool audio_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        edited |= ImGui::Checkbox("Is play on start", &is_play_on_start);

        edited |= ImGui::Checkbox("Is surrounded", &is_surrounded);
        edited |= ImGui::DragFloat("Max distance", &max_distance, 0.1f);
        edited |= ImGui::DragFloat2(
            "Channels intensity", &channels_intensity.x, 0.01f, 0.0f, 1.0f);
        edited |= ImGui::DragFloat("Volume", &volume, 0.1f, 0.0f, 1.0f);
        if (continuous_object)
        {
            edited |=
                ImGui::Checkbox("Is looped", &continuous_object->is_looped);
        }

        auto new_audio_res =
            continuous_object ? continuous_object->get_res() : nullptr;
        if (imgui::edit_resource("Audio resource", &new_audio_res))
        {
            bool is_looped = false;
            if (continuous_object)
            {
                is_looped = continuous_object->is_looped;
                audio::destroy_continuous_object(continuous_object);
            }
            continuous_object = audio::create_continuous_object(new_audio_res);
            if (continuous_object)
            {
                continuous_object->set_volume(volume * channels_intensity);
                continuous_object->is_looped = is_looped;
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

#endif
        return edited;
    }

} // namespace Kengine