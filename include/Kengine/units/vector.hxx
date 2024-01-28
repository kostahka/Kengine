#pragma once

#include "box2d/b2_math.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace Kengine
{
    struct vec3 : public glm::vec3
    {

        constexpr vec3(float x, float y, float z)
            : glm::vec3(x, y, z){};
        constexpr vec3(const b2Vec3& v)
            : glm::vec3(v.x, v.y, v.z){};
        constexpr vec3(const b2Vec2& v)
            : glm::vec3(v.x, v.y, 0.f){};

        operator b2Vec3() const { return { x, y, z }; };

        operator b2Vec2() const { return { x, y }; };
    };

    typedef vec3 vec;

    struct vec2 : public glm::vec2
    {
        constexpr vec2(float x, float y)
            : glm::vec2(x, y){};
        constexpr vec2(const b2Vec2& v)
            : glm::vec2(v.x, v.y){};

        operator b2Vec2() const { return { x, y }; };
    };

    typedef glm::vec4 vec4;

    typedef glm::ivec3 ivec3;
    typedef glm::ivec2 ivec2;
    typedef glm::ivec4 ivec4;

    typedef glm::uvec3 uvec3;
    typedef glm::uvec2 uvec2;
    typedef glm::uvec4 uvec4;

} // namespace Kengine