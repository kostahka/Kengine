#include "Kengine/components/physics-component.hxx"

#include "Kengine/components/component-info.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/physics/physics.hxx"

#include "box2d/b2_body.h"
#include "box2d/b2_chain_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_edge_shape.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "imgui.h"

#include <unordered_map>

namespace Kengine
{
    static std::unordered_map<b2Shape::Type, const char*> b2_shape_types = {
        {b2Shape::Type::e_circle,   "Circle" },
        { b2Shape::Type::e_chain,   "Chain"  },
        { b2Shape::Type::e_edge,    "Edge"   },
        { b2Shape::Type::e_polygon, "Polygon"},
    };

    body_shape::body_shape()
        : shape(nullptr)
    {
    }

    body_shape::body_shape(b2Shape* shape)
        : shape(shape)
    {
    }

    void body_shape::reset(b2Shape* shape)
    {
        if (this->shape)
            delete this->shape;

        this->shape = shape;
    }

    std::size_t body_shape::serialize(std::ostream& os) const
    {
        KENGINE_ASSERT(shape, "NULL shape");
        std::size_t size = 0;

        b2Shape::Type shape_type = shape->GetType();
        size += serialization::write(os, shape_type);
        size += serialization::write(os, shape->m_radius);

        if (shape_type == b2Shape::Type::e_circle)
        {
            const b2CircleShape* circle = static_cast<b2CircleShape*>(shape);

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
        float         m_radius   = 0;
        size += serialization::read(is, shape_type);
        size += serialization::read(is, m_radius);

        if (shape_type == b2Shape::Type::e_circle)
        {
            b2CircleShape* circle = new b2CircleShape();

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

            if (vertices_count > 0)
            {
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
            }

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

            int32 vertices_count = 0;

            size += serialization::read(is, vertices_count);

            if (vertices_count > 0)
            {
                b2Vec2* vertices = new b2Vec2[vertices_count];
                for (auto i = 0; i < vertices_count; ++i)
                {
                    float x, y;
                    size += serialization::read(is, x);
                    size += serialization::read(is, y);

                    vertices[i].Set(x, y);
                }

                polygon->Set(vertices, vertices_count);

                delete[] vertices;
            }

            shape = polygon;
        }

        shape->m_radius = m_radius;

        return size;
    }

    std::size_t body_shape::serialize_size() const
    {
        std::size_t size = 0;

        b2Shape::Type shape_type = shape->GetType();
        size += serialization::size(shape_type);
        size += serialization::size(shape->m_radius);

        if (shape_type == b2Shape::Type::e_circle)
        {
            const b2CircleShape* circle = static_cast<b2CircleShape*>(shape);

            size += serialization::size(circle->m_p.x);
            size += serialization::size(circle->m_p.y);
        }
        else if (shape_type == b2Shape::Type::e_chain)
        {
            const b2ChainShape* chain = static_cast<b2ChainShape*>(shape);

            size += serialization::size(chain->m_prevVertex.x);
            size += serialization::size(chain->m_prevVertex.y);
            size += serialization::size(chain->m_nextVertex.x);
            size += serialization::size(chain->m_nextVertex.y);

            size += serialization::size(chain->m_count);
            for (auto i = 0; i < chain->m_count; ++i)
            {
                size += serialization::size(chain->m_vertices[i].x);
                size += serialization::size(chain->m_vertices[i].y);
            }
        }
        else if (shape_type == b2Shape::Type::e_edge)
        {
            const b2EdgeShape* edge = static_cast<b2EdgeShape*>(shape);

            size += serialization::size(edge->m_oneSided);
            size += serialization::size(edge->m_vertex1.x);
            size += serialization::size(edge->m_vertex1.y);
            size += serialization::size(edge->m_vertex2.x);
            size += serialization::size(edge->m_vertex2.y);

            if (edge->m_oneSided)
            {
                size += serialization::size(edge->m_vertex0.x);
                size += serialization::size(edge->m_vertex0.y);
                size += serialization::size(edge->m_vertex3.x);
                size += serialization::size(edge->m_vertex3.y);
            }
        }
        else if (shape_type == b2Shape::Type::e_polygon)
        {
            const b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);

            size += serialization::size(polygon->m_count);
            for (auto i = 0; i < polygon->m_count; ++i)
            {
                size += serialization::size(polygon->m_vertices[i].x);
                size += serialization::size(polygon->m_vertices[i].y);
            }
        }

        return size;
    }

    b2Shape* body_shape::copy() const
    {
        if (shape)
        {
            b2Shape::Type shape_type = shape->GetType();

            if (shape_type == b2Shape::Type::e_circle)
            {
                const b2CircleShape* circle =
                    static_cast<b2CircleShape*>(shape);

                auto copy_circle = new b2CircleShape(*circle);
                return copy_circle;
            }
            else if (shape_type == b2Shape::Type::e_chain)
            {
                const b2ChainShape* chain = static_cast<b2ChainShape*>(shape);

                auto copy_chain = new b2ChainShape();
                copy_chain->CreateChain(chain->m_vertices,
                                        chain->m_count,
                                        chain->m_prevVertex,
                                        chain->m_nextVertex);

                return copy_chain;
            }
            else if (shape_type == b2Shape::Type::e_edge)
            {
                const b2EdgeShape* edge      = static_cast<b2EdgeShape*>(shape);
                auto               copy_edge = new b2EdgeShape();

                if (edge->m_oneSided)
                {
                    copy_edge->SetOneSided(edge->m_vertex0,
                                           edge->m_vertex1,
                                           edge->m_vertex2,
                                           edge->m_vertex3);
                }
                else
                {
                    copy_edge->SetTwoSided(edge->m_vertex1, edge->m_vertex2);
                }

                return copy_edge;
            }
            else if (shape_type == b2Shape::Type::e_polygon)
            {
                const b2PolygonShape* polygon =
                    static_cast<b2PolygonShape*>(shape);
                auto copy_polygon = new b2PolygonShape();

                copy_polygon->Set(copy_polygon->m_vertices,
                                  copy_polygon->m_count);

                return copy_polygon;
            }
        }

        return nullptr;
    }

    bool body_shape::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        if (shape)
        {
            ImGui::PushID(this);
            b2Shape::Type shape_type = shape->GetType();

            if (shape_type == b2Shape::Type::e_circle)
            {
                b2CircleShape* circle = static_cast<b2CircleShape*>(shape);

                edited = edited ||
                         ImGui::DragFloat2("Position", (float*)&circle->m_p);
                edited =
                    edited || ImGui::DragFloat("Radius", &circle->m_radius);
            }
            else if (shape_type == b2Shape::Type::e_chain)
            {
                b2ChainShape* chain = static_cast<b2ChainShape*>(shape);

                ImGui::Text("Not implemented");
            }
            else if (shape_type == b2Shape::Type::e_edge)
            {
                b2EdgeShape* edge = static_cast<b2EdgeShape*>(shape);

                ImGui::Text("Not implemented");
            }
            else if (shape_type == b2Shape::Type::e_polygon)
            {
                b2PolygonShape* polygon = static_cast<b2PolygonShape*>(shape);

                static float width = 0, height = 0;
                ImGui::DragFloat("Box width", &width, 0.1f);
                ImGui::DragFloat("Box height", &height, 0.1f);
                if (ImGui::Button("Set as box"))
                {
                    polygon->SetAsBox(width / 2, height / 2);
                    edited = true;
                }
            }
            ImGui::PopID();
        }
#endif
        return edited;
    }

    body_fixture::body_fixture()
        : fixture(nullptr)
    {
    }

    body_fixture::body_fixture(b2Fixture* fixture)
        : fixture(fixture)
        , shape(fixture->GetShape())
    {
    }

    b2FixtureDef body_fixture::get_copy_definition() const
    {
        b2FixtureDef copy_definition;
        if (fixture)
        {
            copy_definition.density     = fixture->GetDensity();
            copy_definition.filter      = fixture->GetFilterData();
            copy_definition.friction    = fixture->GetFriction();
            copy_definition.isSensor    = fixture->IsSensor();
            copy_definition.restitution = fixture->GetRestitution();
            copy_definition.restitutionThreshold =
                fixture->GetRestitutionThreshold();
            copy_definition.shape = shape.copy();
        }
        return copy_definition;
    }

    std::size_t body_fixture::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

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

        b2FixtureDef fixture_def{};

        size += shape.deserialize(is);
        fixture_def.shape = shape.get();

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

        shape.reset(fixture->GetShape());

        return size;
    }

    std::size_t body_fixture::serialize_size() const
    {
        std::size_t size = 0;

        size += shape.serialize_size();
        size += serialization::size(fixture->GetFriction());
        size += serialization::size(fixture->GetRestitution());
        size += serialization::size(fixture->GetRestitutionThreshold());
        size += serialization::size(fixture->GetDensity());
        size += serialization::size(fixture->IsSensor());

        auto filter = fixture->GetFilterData();
        size += serialization::size(filter.categoryBits);
        size += serialization::size(filter.groupIndex);
        size += serialization::size(filter.maskBits);

        return size;
    }

    bool body_fixture::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        if (fixture)
        {
            ImGui::PushID(this);
            float friction = fixture->GetFriction();
            if (ImGui::DragFloat("Friction", &friction, 0.1f))
            {
                fixture->SetFriction(friction);
                edited = true;
            }
            float restitution = fixture->GetRestitution();
            if (ImGui::DragFloat("Restitution", &restitution, 0.1f))
            {
                fixture->SetRestitution(restitution);
                edited = true;
            }
            float restitution_threshold = fixture->GetRestitutionThreshold();
            if (ImGui::DragFloat(
                    "Restitution threshold", &restitution_threshold, 0.1f))
            {
                fixture->SetRestitutionThreshold(restitution_threshold);
                edited = true;
            }
            float density = fixture->GetDensity();
            if (ImGui::DragFloat("Density", &density, 0.1f))
            {
                fixture->SetDensity(density);
                edited = true;
            }
            bool is_sensor = fixture->IsSensor();
            if (ImGui::Checkbox("Is sensor", &is_sensor))
            {
                fixture->SetSensor(is_sensor);
                edited = true;
            }
            ImGui::BulletText("Shape");
            ImGui::BeginChild("Shape",
                              { 0, 0 },
                              ImGuiChildFlags_AutoResizeX |
                                  ImGuiChildFlags_AutoResizeY |
                                  ImGuiChildFlags_Border);

            edited = edited || shape.imgui_editable_render();

            ImGui::EndChild();
            ImGui::PopID();
        }
#endif
        return edited;
    }

    physics_component::physics_component()
        : component(name)
        , body(nullptr)
        , world(nullptr)
    {
    }

    physics_component::physics_component(b2World* world)
        : component(name)
        , world(world)
    {
        static b2BodyDef defaultBodyDef{};
        body = world->CreateBody(&defaultBodyDef);
    }

    physics_component::physics_component(b2World*         world,
                                         const b2BodyDef& definition)
        : component(name)
        , world(world)
    {
        body = world->CreateBody(&definition);
    }

    physics_component::physics_component(physics_component& other,
                                         b2World*           world)
        : component(name)
        , body(nullptr)
        , world(world)
    {
        copy_from(other);
    }

    physics_component::physics_component(physics_component&& other)
        : component(name)
        , body(nullptr)
    {
        std::swap(body, other.body);
        std::swap(world, other.world);
        std::swap(fixtures, other.fixtures);
    }

    physics_component& physics_component::operator=(physics_component& other)
    {
        if (body)
        {
            if (world)
            {
                world->DestroyBody(body);
            }
            body = nullptr;
        }
        fixtures.clear();
        copy_from(other);
        return *this;
    }

    physics_component& physics_component::operator=(physics_component&& other)
    {
        std::swap(body, other.body);
        std::swap(world, other.world);
        std::swap(fixtures, other.fixtures);
        return *this;
    }

    body_fixture physics_component::create_fixture(
        const b2FixtureDef& definition)
    {
        KENGINE_ASSERT(body, "Physics body is null");
        auto fixture = body->CreateFixture(&definition);
        fixtures.push_back(body_fixture(fixture));
        return fixture;
    }

    body_fixture physics_component::create_fixture(const b2Shape* shape,
                                                   float          density)
    {
        KENGINE_ASSERT(body, "Physics body is null");
        auto fixture = body->CreateFixture(shape, density);
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
        if (body)
            world->DestroyBody(body);
    }

    void physics_component::set_world(b2World* world)
    {
        if (world)
            this->world = world;
    }

    std::size_t physics_component::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        KENGINE_ASSERT(body, "Physics body is null");
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

    std::size_t physics_component::serialize_size() const
    {
        std::size_t size = 0;

        KENGINE_ASSERT(body, "Physics body is null");
        size += serialization::size(body->GetType());
        auto pos = body->GetPosition();
        size += serialization::size(pos.x);
        size += serialization::size(pos.y);
        size += serialization::size(body->GetAngle());
        auto linearVelocity = body->GetLinearVelocity();
        size += serialization::size(linearVelocity.x);
        size += serialization::size(linearVelocity.y);
        size += serialization::size(body->GetAngularVelocity());
        size += serialization::size(body->GetLinearDamping());
        size += serialization::size(body->GetAngularDamping());
        size += serialization::size(body->IsSleepingAllowed());
        size += serialization::size(body->IsAwake());
        size += serialization::size(body->IsFixedRotation());
        size += serialization::size(body->IsBullet());
        size += serialization::size(body->IsEnabled());
        size += serialization::size(body->GetGravityScale());

        int fixture_count = static_cast<int>(fixtures.size());
        size += serialization::size(fixture_count);
        for (auto i = 0; i < fixture_count; ++i)
        {
            size += fixtures[i].serialize_size();
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

        body = world->CreateBody(&body_def);

        int fixture_count = 0;
        size += serialization::read(is, fixture_count);
        fixtures.resize(fixture_count);
        for (auto i = 0; i < fixture_count; ++i)
        {
            size += fixtures[i].deserialize(is, body);
        }

        return size;
    }

    bool physics_component::imgui_editable_render()
    {
        bool edited = false;
#ifdef KENGINE_IMGUI
        ImGui::PushID(this);
        if (body)
        {
            vec2  position = body->GetPosition();
            float angle    = body->GetAngle();
            if (ImGui::DragFloat2("Position", (float*)&position, 0.1f) ||
                ImGui::DragFloat("Angle", &angle, 0.1f))
            {
                body->SetTransform(position, angle);
                edited = true;
            }
            vec2 linear_velocity = body->GetLinearVelocity();
            if (ImGui::DragFloat2(
                    "Linear velocity", (float*)&linear_velocity, 0.1f))
            {
                body->SetLinearVelocity(linear_velocity);
                edited = true;
            }
            float angular_velocity = body->GetAngularVelocity();
            if (ImGui::DragFloat("Angular velocity", &angular_velocity, 0.1f))
            {
                body->SetAngularVelocity(angular_velocity);
                edited = true;
            }
            float linear_damping = body->GetLinearDamping();
            if (ImGui::DragFloat("Linear damping", &linear_damping, 0.1f))
            {
                body->SetLinearDamping(linear_damping);
                edited = true;
            }
            float angular_damping = body->GetAngularDamping();
            if (ImGui::DragFloat("Angular damping", &angular_damping, 0.1f))
            {
                body->SetAngularDamping(angular_damping);
                edited = true;
            }
            float gravity_scale = body->GetGravityScale();
            if (ImGui::DragFloat("Gravity scale", &gravity_scale, 0.1f))
            {
                body->SetGravityScale(gravity_scale);
                edited = true;
            }
            bool allow_sleep = body->IsSleepingAllowed();
            if (ImGui::Checkbox("Allow sleep", &allow_sleep))
            {
                body->SetSleepingAllowed(allow_sleep);
                edited = true;
            }
            bool awake = body->IsAwake();
            if (ImGui::Checkbox("Awake", &awake))
            {
                body->SetAwake(awake);
                edited = true;
            }
            bool fixed_rotation = body->IsFixedRotation();
            if (ImGui::Checkbox("Fixed rotation", &fixed_rotation))
            {
                body->SetFixedRotation(fixed_rotation);
                edited = true;
            }
            bool bullet = body->IsBullet();
            if (ImGui::Checkbox("Bullet", &bullet))
            {
                body->SetBullet(bullet);
                edited = true;
            }
            bool enabled = body->IsEnabled();
            if (ImGui::Checkbox("Enabled", &enabled))
            {
                body->SetEnabled(enabled);
                edited = true;
            }

            static int      delete_fixture   = -1;
            static int      recreate_fixture = -1;
            static b2Shape* new_shape        = nullptr;

            for (auto i = 0u; i < fixtures.size(); ++i)
            {
                ImGui::BeginChild(i + 1,
                                  { 0, 0 },
                                  ImGuiChildFlags_AutoResizeX |
                                      ImGuiChildFlags_AutoResizeY |
                                      ImGuiChildFlags_Border);

                // Filter edit
                {
                    b2Filter fixture_filter = fixtures[i]->GetFilterData();
                    bool     filter_edited  = false;
                    filter_edited |=
                        ImGui::InputScalar("Category bits",
                                           ImGuiDataType_U16,
                                           &fixture_filter.categoryBits,
                                           nullptr,
                                           nullptr,
                                           "%04X");
                    filter_edited |=
                        ImGui::InputScalar("Group index",
                                           ImGuiDataType_U16,
                                           &fixture_filter.groupIndex,
                                           nullptr,
                                           nullptr,
                                           "%04X");
                    filter_edited |=
                        ImGui::InputScalar("Mask bits",
                                           ImGuiDataType_U16,
                                           &fixture_filter.maskBits,
                                           nullptr,
                                           nullptr,
                                           "%04X");

                    if (filter_edited)
                    {
                        fixtures[i]->SetFilterData(fixture_filter);
                        edited = true;
                    }
                }

                auto fixture_shape_type = fixtures[i]->GetShape()->GetType();
                auto fixture_shape_name_it =
                    b2_shape_types.find(fixture_shape_type);

                auto fixture_shape_name =
                    fixture_shape_name_it != b2_shape_types.end()
                        ? fixture_shape_name_it->second
                        : "";

                if (ImGui::BeginCombo("Shape type", fixture_shape_name))
                {
                    for (auto& shape_type_it : b2_shape_types)
                    {
                        if (ImGui::Selectable(shape_type_it.second))
                        {
                            switch (shape_type_it.first)
                            {
                                case b2Shape::Type::e_circle:
                                    new_shape = new b2CircleShape();
                                    break;
                                case b2Shape::Type::e_chain:
                                    // new_shape = new b2ChainShape();
                                    break;
                                case b2Shape::Type::e_edge:
                                    // new_shape = new b2EdgeShape();
                                    break;
                                case b2Shape::Type::e_polygon:
                                    new_shape = new b2PolygonShape();
                                    break;
                                default:
                                    new_shape = nullptr;
                                    break;
                            }
                            recreate_fixture = i;
                        }
                    }
                    ImGui::EndCombo();
                }

                edited = edited || fixtures[i].imgui_editable_render();

                if (ImGui::Button("Delete"))
                {
                    delete_fixture = i;
                }
                ImGui::EndChild();
            }

            if (delete_fixture >= 0)
            {
                body->DestroyFixture(fixtures[delete_fixture].get());
                fixtures.erase(fixtures.begin() + delete_fixture);
                edited         = true;
                delete_fixture = -1;
            }

            if (recreate_fixture >= 0 && new_shape != nullptr)
            {
                b2FixtureDef newFixtureDef{};

                newFixtureDef.density =
                    fixtures[recreate_fixture]->GetDensity();
                newFixtureDef.filter =
                    fixtures[recreate_fixture]->GetFilterData();
                newFixtureDef.friction =
                    fixtures[recreate_fixture]->GetFriction();
                newFixtureDef.isSensor = fixtures[recreate_fixture]->IsSensor();
                newFixtureDef.restitution =
                    fixtures[recreate_fixture]->GetRestitution();
                newFixtureDef.restitutionThreshold =
                    fixtures[recreate_fixture]->GetRestitutionThreshold();
                newFixtureDef.shape = new_shape;

                body->DestroyFixture(fixtures[recreate_fixture].get());
                auto new_fixture = body->CreateFixture(&newFixtureDef);
                fixtures[recreate_fixture] = body_fixture(new_fixture);

                delete new_shape;
                new_shape = nullptr;

                edited           = true;
                recreate_fixture = -1;
            }

            if (ImGui::Button("Add fixture"))
            {
                auto new_shape = new b2CircleShape();
                create_fixture(new_shape, 1);
                delete new_shape;
                edited = true;
            }
        }
        ImGui::PopID();
#endif
        return edited;
    }

    void physics_component::copy_from(physics_component& other)
    {
        b2BodyDef copyDefinition;
        copyDefinition.type            = other.body->GetType();
        copyDefinition.position        = other.body->GetPosition();
        copyDefinition.angle           = other.body->GetAngle();
        copyDefinition.linearVelocity  = other.body->GetLinearVelocity();
        copyDefinition.angularVelocity = other.body->GetAngularVelocity();
        copyDefinition.linearDamping   = other.body->GetLinearDamping();
        copyDefinition.angularDamping  = other.body->GetAngularDamping();
        copyDefinition.allowSleep      = other.body->IsSleepingAllowed();
        copyDefinition.awake           = other.body->IsAwake();
        copyDefinition.fixedRotation   = other.body->IsFixedRotation();
        copyDefinition.bullet          = other.body->IsBullet();
        copyDefinition.enabled         = other.body->IsEnabled();
        copyDefinition.gravityScale    = other.body->GetGravityScale();

        body = world->CreateBody(&copyDefinition);

        for (auto& fixture : other.fixtures)
        {
            auto fixture_copy_def = fixture.get_copy_definition();
            auto copy_fixture     = body->CreateFixture(&fixture_copy_def);
            fixtures.push_back(body_fixture(copy_fixture));
            delete fixture_copy_def.shape;
        }
    }

    template <>
    void archive_input::operator()(physics_component& value)
    {
        value.set_world(&(sc.get_world()));
        total_size += serialization::read(is, value);
    }

    component_info physics_component::info{
        [](scene& sc, entt::entity ent)
        { return sc.registry.any_of<physics_component>(ent); },

        [](scene& sc, entt::entity ent) {
            return static_cast<component*>(
                &sc.registry.get<physics_component>(ent));
        },

        [](entt::snapshot& snapshot, archive_output& output)
        { snapshot.get<physics_component>(output); },

        [](entt::snapshot& snapshot, archive_size& output)
        { snapshot.get<physics_component>(output); },

        [](entt::snapshot_loader& snapshot, archive_input& input)
        { snapshot.get<physics_component>(input); },

        [](entt::continuous_loader& snapshot, archive_input& input)
        { snapshot.get<physics_component>(input); },

        [](scene& sc, entt::entity ent)
        { sc.registry.erase<physics_component>(ent); },

        [](scene& sc, entt::entity ent)
        { sc.registry.emplace<physics_component>(ent, &sc.get_world()); },

        [](scene& sc, entt::entity ent)
        { sc.registry.patch<physics_component>(ent); },

        [](scene&                                         sc,
           entt::entity                                   ent,
           component*                                     other,
           std::unordered_map<entt::entity, entt::entity> map)
        {
            sc.registry.emplace<physics_component>(
                ent, *static_cast<physics_component*>(other), &sc.get_world());
        },
    };
} // namespace Kengine