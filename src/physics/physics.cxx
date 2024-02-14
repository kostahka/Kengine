#include "physics.hxx"

#include "../scene/scene-manager.hxx"
#include "Kengine/components/physics-component.hxx"
#include "Kengine/components/transform-component.hxx"
#include "Kengine/scene/scene-manager.hxx"

namespace Kengine::physics
{
    b2World* main_world;

    static int velocity_iterations = 10;
    static int position_iterations = 8;

    void initialize(vec2 gravity)
    {
        main_world = new b2World(gravity);
    }

    void shutdown()
    {
        delete main_world;
    }

    void on_update(int delta_ms) {}
} // namespace Kengine::physics