#pragma once

#include "Kengine/scene/scene.hxx"
#include "Kengine/system/system.hxx"

class player_controller_system : public Kengine::system
{
public:
    static constexpr const char* name = "player_controller_system";

    player_controller_system(Kengine::scene& sc);

    std::size_t serialize(std::ostream& os) const override;
    std::size_t deserialize(std::istream& is) override;
    std::size_t serialize_size() const override;

    void on_event(Kengine::scene&                   sc,
                  const Kengine::event::game_event& g_event) override;
    void on_update(Kengine::scene& sc, int delta_ms) override;

    Kengine::scene& sc;
};