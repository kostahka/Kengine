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

        mat4x4 view;

        void set_projection(float width, float height, float zNear, float zFar);

        inline float get_width() const { return width; }

        inline float get_height() const { return height; }

        inline float get_zNear() const { return zNear; }

        inline float get_zFar() const { return zFar; }

        inline const mat4x4& get_projection() const { return projection; }

    private:
        mat4x4 projection;
        float  width  = 2;
        float  height = 2;
        float  zNear  = -1;
        float  zFar   = 1;
    };
} // namespace Kengine::graphics