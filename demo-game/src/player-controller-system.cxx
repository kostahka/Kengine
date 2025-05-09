#include "player-controller-system.hxx"

#include "player-controller-component.hxx"

#include "Kengine/components/transform-component.hxx"

player_controller_system::player_controller_system(Kengine::scene& sc)
    : Kengine::system(name,
                      Kengine::system_type::system_update_type |
                          Kengine::system_type::system_event_type)
    , sc(sc)
{
}

std::size_t player_controller_system::serialize(std::ostream& os) const
{
    return 0;
}

std::size_t player_controller_system::deserialize(std::istream& is)
{
    return 0;
}

std::size_t player_controller_system::serialize_size() const
{
    return 0;
}

void player_controller_system::on_event(
    Kengine::scene& sc, const Kengine::event::game_event& g_event)
{
    using namespace Kengine::event;
    using namespace Kengine::input;
    if (g_event.g_type == type::keyboard_event)
    {

        Kengine::vec2 move_delta = { 0, 0 };
        if (g_event.keyboard.pressed)
        {
            switch (g_event.keyboard.key)
            {
                case keyboard::key::key_a:
                    move_delta.x += -1;
                    break;
                case keyboard::key::key_d:
                    move_delta.x += 1;
                    break;
                case keyboard::key::key_w:
                    move_delta.y += 1;
                    break;
                case keyboard::key::key_s:
                    move_delta.y += -1;
                    break;

                default:
                    break;
            }
        }

        auto player_controller_view =
            sc.registry.view<player_controller_component>();
        for (auto [ent, play_contr_ent] : player_controller_view.each())
        {
            play_contr_ent.move_dir = move_delta;
        }
    }
}

void player_controller_system::on_update(Kengine::scene& sc, int delta_ms)
{
    auto player_controller_view =
        sc.registry
            .view<Kengine::transform_component, player_controller_component>();
    for (auto [ent, transf_ent, play_contr_ent] : player_controller_view.each())
    {
        transf_ent.transf.position += play_contr_ent.move_dir *
                                      play_contr_ent.move_speed *
                                      static_cast<float>(delta_ms);
    }
}
