#include "audio.hxx"

#include "Kengine/log/log.hxx"
#include "Kengine/resources/audio-resource.hxx"
#include "audio-mix.hxx"

#include <SDL3/SDL_audio.h>

#include <algorithm>
#include <list>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace Kengine::audio
{
    struct one_shot_object;
    class continuous_object_impl;

    static SDL_AudioStream* main_audio_stream;

    static SDL_AudioSpec default_spec{ SDL_AUDIO_S16LE, 2, 44100 };
    static SDL_AudioSpec main_in_spec{};
    static SDL_AudioSpec main_out_spec{};

    static spin_lock audio_lock;

    static std::vector<Uint8> internal_sample_buffer;
    static size_t             internal_buffer_size = 50;

    static std::list<continuous_object_impl> continuous_list;
    static std::list<one_shot_object>        one_shot_list;

    class continuous_object_impl : public continuous_object
    {
    public:
        continuous_object_impl(const res_ptr<audio_resource>& res)

        {
            this->res = res;
            if (this->res)
            {
                this->res->take_data();
            }
        }

        continuous_object_impl(continuous_object_impl& other)
        {
            current_play_index = other.current_play_index;
            volume             = other.volume;
            res                = other.res;
            res->take_data();
        }

        continuous_object_impl(continuous_object_impl&& other)
        {
            current_play_index = other.current_play_index;
            volume             = other.volume;
            std::swap(res, other.res);
        }

        continuous_object_impl& operator=(continuous_object_impl& other)
        {
            if (res)
            {
                res->free_data();
            }
            current_play_index = other.current_play_index;
            volume             = other.volume;
            res                = other.res;
            res->take_data();
            return *this;
        }

        continuous_object_impl& operator=(continuous_object_impl&& other)
        {
            if (res)
            {
                res->free_data();
            }
            current_play_index = other.current_play_index;
            volume             = other.volume;
            std::swap(res, other.res);
            return *this;
        }

        ~continuous_object_impl()
        {
            if (res)
            {
                res->free_data();
            }
        }

        void resume() override { is_playing = true; }

        void restart() override
        {
            is_playing         = true;
            current_play_index = 0;
        }

        void pause() override { is_playing = false; }

        void stop() override
        {
            is_playing         = false;
            current_play_index = 0;
        }

        uint32_t current_play_index = 0;
        bool     is_playing         = false;
        bool     to_destroy         = false;
    };

    struct one_shot_object
    {
        one_shot_object(const res_ptr<audio_resource>& res)
            : res(res)
        {
            if (res)
            {
                this->res->take_data();
            }
        }

        one_shot_object(const res_ptr<audio_resource>& res, float volume)
            : res(res)
        {
            if (volume > 1)
            {
                volume = 1;
            }
            else if (volume < 0)
            {
                volume = 0;
            }
            this->volume = { volume, volume };
            if (res)
            {
                this->res->take_data();
            }
        }

        one_shot_object(const res_ptr<audio_resource>& res, vec2 volume)
            : res(res)
        {
            if (volume.x > 1)
            {
                volume.x = 1;
            }
            else if (volume.x < 0)
            {
                volume.x = 0;
            }
            if (volume.y > 1)
            {
                volume.y = 1;
            }
            else if (volume.y < 0)
            {
                volume.y = 0;
            }

            this->volume = volume;
            if (res)
            {
                this->res->take_data();
            }
        }

        one_shot_object(one_shot_object& other)
        {
            current_play_index = other.current_play_index;
            volume             = other.volume;
            res                = other.res;
            res->take_data();
        }

        one_shot_object(one_shot_object&& other)
        {
            current_play_index = other.current_play_index;
            volume             = other.volume;
            std::swap(res, other.res);
        }

        one_shot_object& operator=(one_shot_object& other)
        {
            if (res)
            {
                res->free_data();
            }
            current_play_index = other.current_play_index;
            volume             = other.volume;
            res                = other.res;
            res->take_data();
            return *this;
        }

        one_shot_object& operator=(one_shot_object&& other)
        {
            if (res)
            {
                res->free_data();
            }
            current_play_index = other.current_play_index;
            volume             = other.volume;
            std::swap(res, other.res);
            return *this;
        }

        ~one_shot_object()
        {
            if (res)
            {
                res->free_data();
            }
        }

        uint32_t                current_play_index = 0;
        vec2                    volume             = { 1, 1 };
        res_ptr<audio_resource> res                = nullptr;
    };

    continuous_object::continuous_object() {}

    std::string_view get_sound_format_name(uint16_t format_value)

    {
        static const std::unordered_map<int, std::string_view> format = {
            {SDL_AUDIO_U8,     "AUDIO_U8"   },
            { SDL_AUDIO_S8,    "AUDIO_S8"   },
            { SDL_AUDIO_S16LE, "AUDIO_S16LE"},
            { SDL_AUDIO_S16BE, "AUDIO_S16BE"},
            { SDL_AUDIO_S32LE, "AUDIO_S32LE"},
            { SDL_AUDIO_S32BE, "AUDIO_S32BE"},
            { SDL_AUDIO_F32LE, "AUDIO_F32LE"},
            { SDL_AUDIO_F32BE, "AUDIO_F32BE"},
        };

        auto it = format.find(format_value);
        return it->second;
    }

    std::size_t get_sound_format_size(uint16_t format_value)
    {
        static const std::unordered_map<int, std::size_t> format = {
            {SDL_AUDIO_U8,     1},
            { SDL_AUDIO_S8,    1},
            { SDL_AUDIO_S16LE, 2},
            { SDL_AUDIO_S16BE, 2},
            { SDL_AUDIO_S32LE, 4},
            { SDL_AUDIO_S32BE, 4},
            { SDL_AUDIO_F32LE, 4},
            { SDL_AUDIO_F32BE, 4},
        };

        auto it = format.find(format_value);
        return it->second;
    }

    static int (*mix_audio_format_fp)(
        Uint8*, const Uint8*, SDL_AudioFormat, Uint32, vec2) = nullptr;

    int mix_audio_format_one_channel(Uint8*          dst,
                                     const Uint8*    src,
                                     SDL_AudioFormat format,
                                     Uint32          len,
                                     vec2            volume)
    {
        int final_volume =
            static_cast<int>(((volume.x + volume.y) / 2) * SDL_MIX_MAXVOLUME);
        return SDL_MixAudioFormat(dst, src, format, len, final_volume);
    }

    void audio_callback(void*            userdata,
                        SDL_AudioStream* stream,
                        int              additional_amount,
                        int              total_amount)
    {
        std::lock_guard lock(audio_lock);

        if (additional_amount > internal_buffer_size)
        {
            while (additional_amount > internal_buffer_size)
            {
                internal_buffer_size *= 2;
            }
            internal_sample_buffer.resize(internal_buffer_size);
        }

        std::fill_n(internal_sample_buffer.begin(), additional_amount, 0);

        for (auto one_shot_it = one_shot_list.begin();
             one_shot_it != one_shot_list.end();)
        {
            std::lock_guard lock_sound(one_shot_it->res->data_lock);

            uint8_t* current_stream = &internal_sample_buffer[0];
            uint32_t rest           = one_shot_it->res->get_length() -
                            one_shot_it->current_play_index;
            uint8_t* current_buff =
                &one_shot_it->res->get_data()[one_shot_it->current_play_index];

            if (rest <= static_cast<uint32_t>(additional_amount))
            {
                mix_audio_format_fp(current_stream,
                                    current_buff,
                                    one_shot_it->res->get_spec().fmt,
                                    rest,
                                    one_shot_it->volume);
                one_shot_list.erase(one_shot_it);
                continue;
            }
            else
            {
                mix_audio_format_fp(current_stream,
                                    current_buff,
                                    one_shot_it->res->get_spec().fmt,
                                    static_cast<uint32_t>(additional_amount),
                                    one_shot_it->volume);
                one_shot_it->current_play_index +=
                    static_cast<uint32_t>(additional_amount);
                ++one_shot_it;
            }
        }
        for (auto cont_it = continuous_list.begin();
             cont_it != continuous_list.end();)
        {
            auto audio_res = cont_it->get_res();
            if (cont_it->is_playing && audio_res)
            {
                std::lock_guard lock_sound(audio_res->data_lock);

                uint32_t current_stream_index = 0;
                while (current_stream_index != additional_amount)
                {
                    uint8_t* current_stream =
                        &internal_sample_buffer[current_stream_index];
                    uint32_t rest =
                        audio_res->get_length() - cont_it->current_play_index;
                    uint8_t* current_buff =
                        &audio_res->get_data()[cont_it->current_play_index];

                    if (rest <= static_cast<uint32_t>(additional_amount) -
                                    current_stream_index)
                    {
                        mix_audio_format_fp(current_stream,
                                            current_buff,
                                            audio_res->get_spec().fmt,
                                            rest,
                                            cont_it->get_volume());
                        cont_it->current_play_index = 0;
                        if (!cont_it->is_looped)
                        {
                            cont_it->is_playing = false;
                            break;
                        }
                        current_stream_index += rest;
                    }
                    else
                    {
                        mix_audio_format_fp(
                            current_stream,
                            current_buff,
                            audio_res->get_spec().fmt,
                            static_cast<uint32_t>(additional_amount),
                            cont_it->get_volume());
                        cont_it->current_play_index += static_cast<uint32_t>(
                            additional_amount - current_stream_index);
                        current_stream_index += static_cast<uint32_t>(
                            additional_amount - current_stream_index);
                    }
                }
            }
            if (cont_it->to_destroy)
            {
                continuous_list.erase(cont_it);
            }
            else
            {
                cont_it++;
            }
        }
        SDL_PutAudioStreamData(main_audio_stream,
                               internal_sample_buffer.data(),
                               additional_amount);
    }

    void play_one_shot(res_ptr<audio_resource>& res, float volume)
    {
        std::lock_guard<spin_lock> lock(audio_lock);
        one_shot_list.push_back(one_shot_object(res, volume));
    }

    void play_one_shot(res_ptr<audio_resource>& res, vec2 volume)
    {
        std::lock_guard<spin_lock> lock(audio_lock);
        one_shot_list.push_back(one_shot_object(res, volume));
    }

    bool initialize()
    {
        std::lock_guard<spin_lock> lock(audio_lock);
        internal_sample_buffer.resize(internal_buffer_size);
        main_audio_stream =
            SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_OUTPUT,
                                      &default_spec,
                                      &audio_callback,
                                      nullptr);
        SDL_GetAudioStreamFormat(
            main_audio_stream, &main_in_spec, &main_out_spec);
        if (main_in_spec.channels == 2)
        {
            mix_audio_format_fp = &mix_audio_format_two_channel;
        }
        else
        {
            mix_audio_format_fp = &mix_audio_format_one_channel;
        }

        SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(main_audio_stream));
        return true;
    }

    void shutdown()
    {
        std::lock_guard<spin_lock> lock(audio_lock);
        SDL_PauseAudioDevice(SDL_GetAudioStreamDevice(main_audio_stream));
        SDL_DestroyAudioStream(main_audio_stream);
        main_audio_stream = nullptr;
        one_shot_list.clear();
    }

    spec get_current_out_spec()
    {
        return { main_out_spec.format,
                 main_out_spec.channels,
                 main_out_spec.freq };
    }

    spec get_current_in_spec()
    {
        return { main_in_spec.format,
                 main_in_spec.channels,
                 main_in_spec.freq };
    }

    continuous_object* create_continuous_object(
        const res_ptr<audio_resource>& res)
    {
        if (res)
        {
            std::lock_guard<spin_lock> lock(audio_lock);
            auto new_obj = &continuous_list.emplace_back(res);
            return new_obj;
        }
        return nullptr;
    }

    void destroy_continuous_object(continuous_object* obj)
    {
        std::lock_guard<spin_lock> lock(audio_lock);
        static_cast<continuous_object_impl*>(obj)->to_destroy = true;
    }
} // namespace Kengine::audio
