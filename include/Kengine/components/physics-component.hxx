#pragma once

#include "Kengine/components/component.hxx"

#include "box2d/b2_body.h"

namespace Kengine
{
    class body_shape : public imgui::editable
    {
    public:
        body_shape();
        body_shape(b2Shape* shape);

        std::size_t serialize(std::ostream& os) const;
        std::size_t deserialize(std::istream& is);

        bool imgui_editable_render() override;

        inline b2Shape* get() { return shape; }

        inline const b2Shape* get() const { return shape; }

        inline b2Shape& operator*() { return *shape; }

        inline const b2Shape& operator*() const { return *shape; }

        inline b2Shape* operator->() { return shape; }

        inline const b2Shape* operator->() const { return shape; }

    private:
        b2Shape* shape;
    };

    class body_fixture : public imgui::editable
    {
    public:
        body_fixture();
        body_fixture(b2Fixture* fixture);

        std::size_t serialize(std::ostream& os) const;
        std::size_t deserialize(std::istream& is, b2Body* body);

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

    struct physics_component : public component
    {
        static constexpr auto name = "physics_component";

        physics_component();
        physics_component(const b2BodyDef& definition);

        physics_component(physics_component& other) = delete;
        physics_component(physics_component&& other);

        physics_component& operator=(physics_component& other) = delete;
        physics_component& operator=(physics_component&& other);

        ~physics_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

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
        b2Body*                   body;
        std::vector<body_fixture> fixtures;
    };
} // namespace Kengine