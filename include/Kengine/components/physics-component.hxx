#pragma once

#include "Kengine/components/component.hxx"

#include "box2d/b2_body.h"

namespace Kengine
{
    struct component_info;

    class E_DECLSPEC body_shape : public imgui::editable
    {
    public:
        body_shape();
        body_shape(b2Shape* shape);

        std::size_t serialize(std::ostream& os) const;
        std::size_t deserialize(std::istream& is);
        std::size_t serialize_size() const;

        b2Shape* copy() const;

        bool imgui_editable_render() override;

        void reset(b2Shape* shape);

        inline b2Shape* get() { return shape; }

        inline const b2Shape* get() const { return shape; }

        inline b2Shape& operator*() { return *shape; }

        inline const b2Shape& operator*() const { return *shape; }

        inline b2Shape* operator->() { return shape; }

        inline const b2Shape* operator->() const { return shape; }

    private:
        b2Shape* shape;
    };

    class E_DECLSPEC body_fixture : public imgui::editable
    {
    public:
        body_fixture();
        body_fixture(b2Fixture* fixture);

        b2FixtureDef get_copy_definition() const;

        std::size_t serialize(std::ostream& os) const;
        std::size_t deserialize(std::istream& is, b2Body* body);
        std::size_t serialize_size() const;

        bool imgui_editable_render() override;

        inline b2Fixture* get() { return fixture; }

        inline const b2Fixture* get() const { return fixture; }

        inline b2Fixture& operator*() { return *fixture; }

        inline const b2Fixture& operator*() const { return *fixture; }

        inline b2Fixture* operator->() { return fixture; }

        inline const b2Fixture* operator->() const { return fixture; }

        inline bool operator==(const body_fixture& other) const
        {
            return fixture == other.fixture;
        }

    private:
        b2Fixture* fixture;
        body_shape shape;
    };

    struct E_DECLSPEC physics_component : public component
    {
        static constexpr auto name = "physics_component";
        static component_info info;

        physics_component();
        physics_component(b2World* world);
        physics_component(b2World* world, const b2BodyDef& definition);

        physics_component(physics_component& other) = delete;
        physics_component(physics_component& other, b2World* world);
        physics_component(physics_component&& other);

        void set_world(b2World* world);

        physics_component& operator=(physics_component& other);
        physics_component& operator=(physics_component&& other);

        ~physics_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        body_fixture create_fixture(const b2FixtureDef& definition);
        body_fixture create_fixture(const b2Shape* shape, float density);
        void         destroy_fixture(body_fixture);

        inline const std::vector<body_fixture>& get_fixtures() const
        {
            return fixtures;
        }

        inline b2Body& operator*() { return *body; }

        inline const b2Body& operator*() const { return *body; }

        inline b2Body* operator->() { return body; }

        inline const b2Body* operator->() const { return body; }

        bool imgui_editable_render() override;

    private:
        void copy_from(physics_component& other);

        b2Body*                   body;
        b2World*                  world;
        std::vector<body_fixture> fixtures;
    };
} // namespace Kengine