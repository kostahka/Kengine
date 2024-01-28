#include "scene-manager.hxx"
#include "Kengine/scene/scene-manager.hxx"

#include "Kengine/log/log.hxx"

namespace Kengine::scene_manager
{
    scene* current_scene;

    void set_current_scene(scene* sc)
    {
        KENGINE_ASSERT(sc, "Do not set NULL scene");
        current_scene = sc;
    }

    scene* get_current_scene()
    {
        KENGINE_ASSERT(current_scene, "Current scene is NULL!");
        return current_scene;
    }

    void initialize()
    {
        current_scene = new scene();
    }

    void shutdown() {}

} // namespace Kengine::scene_manager