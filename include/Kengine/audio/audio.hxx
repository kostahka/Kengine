#pragma once

#include "Kengine/audio/specs.hxx"
#include "Kengine/resources/audio-resource.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/units/vector.hxx"

#include <cstdint>
#include <memory>
#include <string_view>

namespace Kengine::audio
{
    class E_DECLSPEC continuous_object
    {
    public:
        continuous_object(continuous_object&)  = delete;
        continuous_object(continuous_object&&) = delete;

        continuous_object& operator=(continuous_object&)  = delete;
        continuous_object& operator=(continuous_object&&) = delete;

        virtual void resume()  = 0;
        virtual void restart() = 0;
        virtual void pause()   = 0;
        virtual void stop()    = 0;

        bool is_looped = false;

        inline res_ptr<audio_resource>& get_res() { return res; }

        inline vec2 get_volume() const { return volume; }

        inline void set_volume(vec2 v)
        {
            if (v.x > 1)
            {
                v.x = 1;
            }
            else if (v.x < 0)
            {
                v.x = 0;
            }
            if (v.y > 1)
            {
                v.y = 1;
            }
            else if (v.y < 0)
            {
                v.y = 0;
            }
            volume = v;
        }

    protected:
        continuous_object();

        res_ptr<audio_resource> res;
        vec2                    volume = { 1, 1 };
    };

    E_DECLSPEC void play_one_shot(res_ptr<audio_resource>& res, float volume);

    E_DECLSPEC void play_one_shot(res_ptr<audio_resource>& res, vec2 volume);

    E_DECLSPEC continuous_object* create_continuous_object(
        const res_ptr<audio_resource>& res);
    E_DECLSPEC void destroy_continuous_object(continuous_object* obj);

    E_DECLSPEC spec get_current_out_spec();
    E_DECLSPEC spec get_current_in_spec();
} // namespace Kengine::audio
