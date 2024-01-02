#pragma once

#include "box2d/b2_math.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Kengine
{
    typedef glm::vec3 vec;
    typedef glm::vec3 vec3;
    // struct vec : public glm::vec3
    // {
    //     vec(const b2Vec3& v)
    //         : glm::vec3(v.x, v.y, v.z){};

    //     inline operator b2Vec3() { return { x, y, z }; };
    // };

    typedef glm::vec2 vec2;
    // struct vec2 : public glm::vec2
    // {
    //     vec2(const b2Vec2& v)
    //         : glm::vec2(v.x, v.y){};

    //     inline operator b2Vec2() { return { x, y }; };
    // };

    typedef glm::vec4 vec4;

    typedef glm::ivec3 ivec3;
    typedef glm::ivec2 ivec2;
    typedef glm::ivec4 ivec4;

    typedef glm::uvec3 uvec3;
    typedef glm::uvec2 uvec2;
    typedef glm::uvec4 uvec4;

} // namespace Kengine