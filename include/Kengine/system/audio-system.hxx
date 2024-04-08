#pragma once

#include "Kengine/scene/scene.hxx"
#include "Kengine/system/system.hxx"

#include "entt/entt.hpp"

namespace Kengine
{

    class audio_system : public system
    {
    public:
        static constexpr auto name = "audio_system";

        audio_system(scene& sc);
        ~audio_system();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        void on_start(scene&) override;

        void on_event(scene&, event::game_event) override;
        void on_render(scene&, int delta_ms) override;
        void on_update(scene&, int delta_ms) override;

    private:
        scene& sc;
    };
} // namespace Kengine