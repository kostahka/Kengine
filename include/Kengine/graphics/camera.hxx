#pragma once

#include "Kengine/serialization/serialization.hxx"
#include "Kengine/units/matrix.hxx"

namespace Kengine::graphics
{
    class camera : public serializable
    {
    public:
        camera();
        ~camera();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        mat4x4 view;

        void set_projection(float height, float zNear, float zFar);
        void set_height(float height);

        inline float get_height() const { return height; }

        inline float get_zNear() const { return zNear; }

        inline float get_zFar() const { return zFar; }

        inline const mat4x4& get_projection() const { return projection; }

        void calculate_projection();

        inline bool is_projection_valid() const { return projection_valid; }

    private:
        mat4x4 projection;
        float  height           = 2;
        float  zNear            = -50;
        float  zFar             = 50;
        bool   projection_valid = false;
    };
} // namespace Kengine::graphics