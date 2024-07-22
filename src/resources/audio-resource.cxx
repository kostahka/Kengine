#include "Kengine/resources/audio-resource.hxx"

#include "../io/file-manager.hxx"
#include "../scene/scene-manager.hxx"
#include "Kengine/audio/audio.hxx"
#include "Kengine/imgui/imgui-edit.hxx"
#include "Kengine/log/log.hxx"

#include "imgui.h"

#include <SDL3/SDL_audio.h>

#include <mutex>

namespace Kengine
{
    audio_resource::audio_resource(std::string_view name)
        : resource(resource_type::audio, name)
    {
    }

    audio_resource::audio_resource(std::filesystem::path f_path,
                                   std::string_view      name)
        : resource(resource_type::audio, f_path, name)
    {
    }

    audio_resource::~audio_resource()
    {
        if (data)
        {
            std::lock_guard<spin_lock> lock(data_lock);
            SDL_free(data);
            data   = nullptr;
            length = 0;
            KENGINE_INFO("Unloaded audio data: {}", r_id.get_string())
        }
    }

    std::size_t audio_resource::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t audio_resource::deserialize(std::istream& is)
    {
        return 0;
    }

    std::size_t audio_resource::serialize_size() const
    {
        return 0;
    }

    bool audio_resource::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);

        edited = edited || imgui::edit_file("Wav file", f_path);

        if (ImGui::Button("Play one shot"))
        {
            auto this_res = static_resource_cast<audio_resource>(
                resource_manager::get_resource(r_id));
            audio::play_one_shot(this_res, 1.0f);
        }

        ImGui::PopID();
#endif
        return edited;
    }

    void audio_resource::load_data()
    {
        std::lock_guard<spin_lock> lock(data_lock);
        if (!f_path.empty())
        {
            auto media_file = file_manager::load_file(
                scene_manager::assets_base_folder / f_path);
            if (media_file)
            {
                file_manager::sdl_io_stream media_io_stream(media_file);
                SDL_AudioSpec               media_spec;
                if (SDL_LoadWAV_IO(media_io_stream.get(),
                                   false,
                                   &media_spec,
                                   &data,
                                   &length) != 0)
                {
                    data   = nullptr;
                    length = 0;
                    KENGINE_ERROR("Failed to load wav file. Error: {}",
                                  SDL_GetError());
                    return;
                }
                spec.fmt       = media_spec.format;
                spec.channels  = media_spec.channels;
                spec.frequency = media_spec.freq;

                auto device_spec = audio::get_current_in_spec();
                if (device_spec.fmt != spec.fmt ||
                    device_spec.channels != spec.channels ||
                    device_spec.frequency != spec.frequency)
                {
                    SDL_AudioSpec device_spec_sdl{ static_cast<SDL_AudioFormat>(
                                                       device_spec.fmt),
                                                   device_spec.channels,
                                                   device_spec.frequency };

                    uint8_t* new_data   = nullptr;
                    int      new_length = 0;

                    if (SDL_ConvertAudioSamples(&media_spec,
                                                data,
                                                length,
                                                &device_spec_sdl,
                                                &new_data,
                                                &new_length) != 0)
                    {
                        SDL_free(data);
                        data   = nullptr;
                        length = 0;
                        KENGINE_ERROR("Failed to convert wav data. Error: {}",
                                      SDL_GetError());
                        return;
                    }
                    SDL_free(data);
                    data   = new_data;
                    length = new_length;
                    spec   = device_spec;
                }

                KENGINE_INFO("Loaded audio data: {}", r_id.get_string());
            }
        }
    }

    void audio_resource::unload_data()
    {
        std::lock_guard<spin_lock> lock(data_lock);
        if (data)
        {
            SDL_free(data);
            data   = nullptr;
            length = 0;
            KENGINE_INFO("Unloaded audio data: {}", r_id.get_string())
        }
    }
} // namespace Kengine