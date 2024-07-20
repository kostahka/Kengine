#pragma once

#include "Kengine/types/vector.hxx"

#include "box2d/b2_body.h"
#include "box2d/b2_math.h"
#include "box2d/b2_world.h"

namespace Kengine::physics
{
    void initialize(vec2 gravity = { 0, 9.81f });
    void shutdown();
    void on_update(int delta_ms);
} // namespace Kengine::physics