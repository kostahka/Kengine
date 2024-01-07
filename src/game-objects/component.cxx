#include "Kengine/game-objects/component.hxx"

namespace Kengine
{
    component::component(component_type type, game_object* game_object)
        : type(type)
        , component_object(game_object)
    {
    }

    component::~component() {}
} // namespace Kengine