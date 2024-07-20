#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/resources/animation-resource.hxx"
#include "Kengine/resources/res-ptr.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/types/rect.hxx"

#include <unordered_map>
#include <vector>

namespace Kengine
{
    struct E_DECLSPEC animation_component : public component
    {
        static constexpr auto name = "animation_component";

        animation_component();
        animation_component(const res_ptr<animation_resource>& anim_res);
        ~animation_component();

        animation_component(animation_component& other);
        animation_component(animation_component&& other);

        animation_component& operator=(animation_component& other);
        animation_component& operator=(animation_component&& other);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        bool is_play_on_start = false;
        bool is_looped        = false;

        void resume();
        void restart();
        void pause();
        void stop();

        int animation_time   = 0;
        int delta_frame_time = 16; // ~60 FPS

        bool reversed   = false;
        bool is_playing = false;

        void set_current_frame(int frame);
        void set_current_animation(string_id id);

        inline rect get_current_uv()
        {
            if (anim_res)
            {
                return anim_res->get_animations()
                    .find(current_animation)
                    ->second[current_frame];
            }
            return {};
        }

        inline int get_current_frame() const { return current_frame; }

        inline string_id get_current_animation() const
        {
            return current_animation;
        }

        inline res_ptr<animation_resource> get_anim_res() const
        {
            return anim_res;
        }

    private:
        int                         current_frame = 0;
        string_id                   current_animation{};
        res_ptr<animation_resource> anim_res = nullptr;
    };

} // namespace Kengine