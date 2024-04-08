#pragma once

#include "Kengine/audio/audio.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/units/vector.hxx"

namespace Kengine
{
    struct audio_component : public component
    {
        static constexpr auto name = "audio_component";

        audio_component();
        audio_component(const res_ptr<audio_resource>& res);
        ~audio_component();

        audio_component(audio_component& other);
        audio_component(audio_component&& other);

        audio_component& operator=(audio_component& other);
        audio_component& operator=(audio_component&& other);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        bool is_play_on_start = false;

        bool  is_surrounded = false;
        float max_distance  = 1;

        void resume();
        void restart();
        void pause();
        void stop();
        void play_one_shot();

        void set_volume(float volume);

        inline float get_volume() const { return volume; }

        void set_channels_intensity(vec2 channels_intensity);

        inline vec2 get_channels_intensity() const
        {
            return channels_intensity;
        }

        inline audio::continuous_object& get_continuous_obj()
        {
            return *continuous_object;
        }

    private:
        vec2                      channels_intensity = { 1, 1 };
        float                     volume             = 1;
        audio::continuous_object* continuous_object  = nullptr;
    };

} // namespace Kengine