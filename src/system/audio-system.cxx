#include "Kengine/system/audio-system.hxx"

#include "../audio/audio.hxx"
#include "Kengine/components/audio-component.hxx"
#include "Kengine/components/camera-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/graphics/camera.hxx"
#include "Kengine/scene/scene.hxx"

namespace Kengine
{
    audio_system::audio_system(scene& sc)
        : system(name, system_update_type)
        , sc(sc)
    {
    }

    audio_system::~audio_system() {}

    void audio_system::on_start(scene& sc)
    {
        auto audio_view = sc.registry.view<audio_component>();
        for (auto [ent, ent_audio] : audio_view.each())
        {
            if (ent_audio.is_play_on_start)
            {
                ent_audio.restart();
            }
        }
    }

    void audio_system::on_event(scene& sc, event::game_event g_event) {}

    void audio_system::on_render(scene& sc, int delta_ms) {}

    void audio_system::on_update(scene& sc, int delta_ms)
    {
        auto audio_view =
            sc.registry.view<transform_component, audio_component>();

        auto  cam_ent   = sc.get_camera_entity();
        vec2  cam_pos   = { 0, 0 };
        float cam_angle = 0;

        if (cam_ent != entt::null &&
            sc.registry.all_of<transform_component>(cam_ent))
        {
            auto cam_transform = sc.registry.get<transform_component>(cam_ent)
                                     .get_world_transform();
            cam_pos   = cam_transform.position;
            cam_angle = cam_transform.angle;
        }

        float cam_cos_angle = glm::cos(cam_angle);
        float cam_sin_angle = glm::cos(cam_angle);

        for (auto [ent, ent_transform, ent_audio] : audio_view.each())
        {
            if (ent_audio.is_surrounded)
            {

                vec2 delta =
                    cam_pos - ent_transform.get_world_transform().position;

                float distance = glm::length(glm::vec2(delta));
                if (distance < ent_audio.max_distance)
                {
                    float channel_orient =
                        cam_cos_angle * delta.x + cam_sin_angle * delta.y;
                    float k = 1.0f - distance / ent_audio.max_distance;
                    vec2  channels_intensity = { k, k };
                    if (channel_orient > 0)
                    {
                        channels_intensity.x *= k;
                    }
                    else
                    {
                        channels_intensity.y *= k;
                    }
                    ent_audio.set_channels_intensity(channels_intensity);
                }
                else
                {
                    ent_audio.set_channels_intensity({ 0, 0 });
                }
            }
        }
    }

    std::size_t audio_system::serialize(std::ostream& os) const
    {
        return 0;
    }

    std::size_t audio_system::deserialize(std::istream& is)
    {
        return 0;
    }

    std::size_t audio_system::serialize_size() const
    {
        return 0;
    }

} // namespace Kengine