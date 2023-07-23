#pragma once

#include "sprite.hxx"
#include <map>

namespace Kengine
{
    struct animation
    {
        std::vector<irect> frames;
        int                delta_time;
        int                current_frame;
    };

    class animation_controller
    {
    public:
        animation_controller(texture_object *,
                             const vec2 &pos,
                             const vec2 &size);

        void add_animation(std::string name,
                           const std::vector<irect> &,
                           int delta_time);
        void set_current_animation(std::string name);

        void set_animation_delta_frame(int delta_frame);

        void play_one_shot();
        void play();
        void stop();

        void draw(std::chrono::duration<int, std::milli> delta_time);

        void set_pos(const vec2 &pos);
        void set_angle(const float angle);
        void set_origin(const vec2 &origin);
        void set_size(const vec2 &size);

    private:
        sprite                     anim_sprite;
        std::map<std::string, int> anim_id;
        std::vector<animation>     animations;
        int                        current_anim;
        bool                       play_anim;
        bool                       play_one_shot_anim;
        int                        last_anim;

        int anim_time;
        int delta_frame;
    };

} // namespace Kengine
