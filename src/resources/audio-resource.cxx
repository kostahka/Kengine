#include "Kengine/resources/audio-resource.hxx"

#include "../io/file-manager.hxx"

#include <SDL3/SDL_audio.h>

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
            SDL_free(data);
            data   = nullptr;
            length = 0;
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
        return false;
    }

    void audio_resource::load_data()
    {
        if (!f_path.empty())
        {
            auto media_file = file_manager::load_file(f_path);
            if (media_file)
            {
                file_manager::sdl_io_stream media_io_stream(media_file);
                SDL_AudioSpec               media_spec;
                SDL_LoadWAV_IO(
                    media_io_stream.get(), false, &media_spec, &data, &length);
                spec.fmt       = media_spec.format;
                spec.channels  = media_spec.channels;
                spec.frequency = media_spec.freq;
            }
        }
    }

    void audio_resource::unload_data()
    {
        if (data)
        {
            SDL_free(data);
            data   = nullptr;
            length = 0;
        }
    }
} // namespace Kengine