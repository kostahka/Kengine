#include "Kengine/components/physics-component.hxx"

#include "Kengine/physics/physics.hxx"
#include "box2d/b2_body.h"
#include "box2d/b2_chain_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_edge_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Kengine
{
    body_shape::body_shape()
        : shape(nullptr)
    {
    }

    body_shape::body_shape(b2Shape* shape)
        : shape(shape)
    {
    }

    std::size_t body_shape::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        b2Shape::Type shape_type = shape->GetType();
        size += serialization::write(os, shape_type);

        if (shape_type == b2Shape::Type::e_circle)
        {
            const b2CircleShape* circle = static_cast<b2CircleShape*>(shape);

            size += serialization::write(os, circle->m_radius);
            size += serialization::write(os, circle->m_p.x);
            size += serialization::write(os, circle->m_p.y);
        }
        else if (shape_type == b2Shape::Type::e_chain)
        {
            const b2ChainShape* chain = static_cast<b2ChainShape*>(shape);

            size += serialization::write(os, chain->m_prevVertex.x);
            size += serialization::write(os, chain->m_prevVertex.y);
            size += serialization::write(os, chain->m_nextVertex.x);
            size += serialization::write(os, chain->m_nextVertex.y);

            size += serialization::write(os, chain->m_count);
            for (auto i = 0; i < chain->m_count; ++i)
            {
                size += serialization::write(os, chain->m_vertices[i].x);
                size += serialization::write(os, chain->m_vertices[i].y);
            }
        }
        else if (shape_type == b2Shape::Type::e_edge)
        {
            const b2EdgeShape* edge = static_cast<b2EdgeShape*>(shape);

            size += serialization::write(os, edge->m_oneSided);
            size += serialization::write(os, edge->m_vertex1.x);
            size += serialization::write(os, edge->m_vertex1.y);
            size += serialization::write(os, edge->m_vertex2.x);
            size += serialization::write(os, edge->m_vertex2.y);

            if (edge->m_oneSided)
            {
                size += serialization::write(os, edge->m_vertex0.x);
                size += serialization::write(os, edge->m_vertex0.y);
                size += serialization::write(os, edge->m_vertex3.x);
                size += serialization::write(os, edge->m_vertex3.y);
            }
        }
        else if (shape_type == b2Shape::Type::e_polygon)
        {
            const b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);

            size += serialization::write(os, polygon->m_count);
            for (auto i = 0; i < polygon->m_count; ++i)
            {
                size += serialization::write(os, polygon->m_vertices[i].x);
                size += serialization::write(os, polygon->m_vertices[i].y);
            }
        }

        return size;
    }

    std::size_t body_shape::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        b2Shape::Type shape_type = b2Shape::Type::e_circle;
        size += serialization::read(is, shape_type);

        if (shape_type == b2Shape::Type::e_circle)
        {
            b2CircleShape* circle = new b2CircleShape();

            size += serialization::read(is, circle->m_radius);
            float x, y;
            size += serialization::read(is, x);
            size += serialization::read(is, y);

            circle->m_p.Set(x, y);

            shape = circle;
        }
        else if (shape_type == b2Shape::Type::e_chain)
        {
            b2ChainShape* chain = new b2ChainShape();

            float prev_x, prev_y, next_x, next_y;

            size += serialization::read(is, prev_x);
            size += serialization::read(is, prev_y);
            size += serialization::read(is, next_x);
            size += serialization::read(is, next_y);

            int32 vertices_count = 0;
            size += serialization::read(is, vertices_count);

            b2Vec2* vertices = new b2Vec2[vertices_count];

            for (auto i = 0; i < vertices_count; ++i)
            {
                float x, y;
                size += serialization::read(is, x);
                size += serialization::read(is, y);
                vertices[i].Set(x, y);
            }

            chain->CreateChain(vertices,
                               vertices_count,
                               { prev_x, prev_y },
                               { next_x, next_y });

            delete[] vertices;

            shape = chain;
        }
        else if (shape_type == b2Shape::Type::e_edge)
        {
            b2EdgeShape* edge = new b2EdgeShape();

            bool  one_sided = false;
            float x1, y1, x2, y2;

            size += serialization::read(is, one_sided);
            size += serialization::read(is, x1);
            size += serialization::read(is, y1);
            size += serialization::read(is, x2);
            size += serialization::read(is, y2);

            if (one_sided)
            {
                float x0, y0, x3, y3;
                size += serialization::read(is, x0);
                size += serialization::read(is, y0);
                size += serialization::read(is, x3);
                size += serialization::read(is, y3);

                edge->SetOneSided(
                    { x0, y0 }, { x1, y1 }, { x2, y2 }, { x3, y3 });
            }
            else
            {
                edge->SetTwoSided({ x1, y1 }, { x2, y2 });
            }

            shape = edge;
        }
        else if (shape_type == b2Shape::Type::e_polygon)
        {
            b2PolygonShape* polygon = new b2PolygonShape();

            int32   vertices_count = 0;
            b2Vec2* vertices       = new b2Vec2[vertices_count];

            size += serialization::read(is, vertices_count);
            for (auto i = 0; i < vertices_count; ++i)
            {
                float x, y;
                size += serialization::read(is, x);
                size += serialization::read(is, y);

                vertices[i].Set(x, y);
            }

            polygon->Set(vertices, vertices_count);

            delete[] vertices;

            shape = polygon;
        }

        return size;
    }

    body_fixture::body_fixture()
        : fixture(nullptr)
    {
    }

    body_fixture::body_fixture(b2Fixture* fixture)
        : fixture(fixture)
    {
    }

    std::size_t body_fixture::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        body_shape shape(fixture->GetShape());

        size += shape.serialize(os);
        size += serialization::write(os, fixture->GetFriction());
        size += serialization::write(os, fixture->GetRestitution());
        size += serialization::write(os, fixture->GetRestitutionThreshold());
        size += serialization::write(os, fixture->GetDensity());
        size += serialization::write(os, fixture->IsSensor());

        auto filter = fixture->GetFilterData();
        size += serialization::write(os, filter.categoryBits);
        size += serialization::write(os, filter.groupIndex);
        size += serialization::write(os, filter.maskBits);

        return size;
    }

    std::size_t body_fixture::deserialize(std::istream& is, b2Body* body)
    {
        std::size_t size = 0;

        body_shape   shape{};
        b2FixtureDef fixture_def{};

        size += shape.deserialize(is);
        size += serialization::read(is, fixture_def.friction);
        size += serialization::read(is, fixture_def.restitution);
        size += serialization::read(is, fixture_def.restitutionThreshold);
        size += serialization::read(is, fixture_def.density);
        size += serialization::read(is, fixture_def.isSensor);

        b2Filter filter{};
        size += serialization::read(is, filter.categoryBits);
        size += serialization::read(is, filter.groupIndex);
        size += serialization::read(is, filter.maskBits);

        fixture_def.filter = filter;

        fixture = body->CreateFixture(&fixture_def);

        return size;
    }

    physics_component::physics_component()
        : component(component_type::physics)
    {
        b2BodyDef emptyBodyDef{};
        body = physics::main_world->CreateBody(&emptyBodyDef);
    }

    physics_component::physics_component(const b2BodyDef& definition)
        : component(component_type::physics)
    {
        body = physics::main_world->CreateBody(&definition);
    }

    body_fixture physics_component::create_fixture(
        const b2FixtureDef& definition)
    {
        auto fixture = body->CreateFixture(&definition);
        fixtures.push_back(body_fixture(fixture));
        return fixture;
    }

    void physics_component::destroy_fixture(body_fixture fixture)
    {
        auto fixture_iterator =
            std::remove(fixtures.begin(), fixtures.end(), fixture);
        if (fixture_iterator != fixtures.end())
        {
            fixtures.erase(fixture_iterator);
        }
    }

    physics_component::~physics_component()
    {
        physics::main_world->DestroyBody(body);
    }

    std::size_t physics_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        size += serialization::write(os, body->GetType());
        auto pos = body->GetPosition();
        size += serialization::write(os, pos.x);
        size += serialization::write(os, pos.y);
        size += serialization::write(os, body->GetAngle());
        auto linearVelocity = body->GetLinearVelocity();
        size += serialization::write(os, linearVelocity.x);
        size += serialization::write(os, linearVelocity.y);
        size += serialization::write(os, body->GetAngularVelocity());
        size += serialization::write(os, body->GetLinearDamping());
        size += serialization::write(os, body->GetAngularDamping());
        size += serialization::write(os, body->IsSleepingAllowed());
        size += serialization::write(os, body->IsAwake());
        size += serialization::write(os, body->IsFixedRotation());
        size += serialization::write(os, body->IsBullet());
        size += serialization::write(os, body->IsEnabled());
        size += serialization::write(os, body->GetGravityScale());

        int fixture_count = static_cast<int>(fixtures.size());
        size += serialization::write(os, fixture_count);
        for (auto i = 0; i < fixture_count; ++i)
        {
            size += fixtures[i].serialize(os);
        }

        return size;
    }

    std::size_t physics_component::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        b2BodyDef body_def{};
        float     x, y;

        size += serialization::read(is, body_def.type);
        size += serialization::read(is, x);
        size += serialization::read(is, y);
        body_def.position.Set(x, y);
        size += serialization::read(is, body_def.angle);
        size += serialization::read(is, x);
        size += serialization::read(is, y);
        body_def.linearVelocity.Set(x, y);
        size += serialization::read(is, body_def.angularVelocity);
        size += serialization::read(is, body_def.linearDamping);
        size += serialization::read(is, body_def.angularDamping);
        size += serialization::read(is, body_def.allowSleep);
        size += serialization::read(is, body_def.awake);
        size += serialization::read(is, body_def.fixedRotation);
        size += serialization::read(is, body_def.bullet);
        size += serialization::read(is, body_def.enabled);
        size += serialization::read(is, body_def.gravityScale);

        body = physics::main_world->CreateBody(&body_def);

        int fixture_count = 0;
        size += serialization::read(is, fixture_count);
        fixtures.resize(fixture_count);
        for (auto i = 0; i < fixture_count; ++i)
        {
            size += fixtures[i].deserialize(is, body);
        }

        return size;
    }
} // namespace Kengine