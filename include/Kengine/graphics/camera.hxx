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

        void set_matrices(const mat4x4& projection, const mat4x4& view);
        void set_projection(const mat4x4& projection);
        void set_view(const mat4x4& view);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        inline const mat4x4& get_projection() const { return projection; }

        inline const mat4x4& get_view() const { return view; };

    private:
        mat4x4 projection;
        mat4x4 view;
    };
} // namespace Kengine::graphics