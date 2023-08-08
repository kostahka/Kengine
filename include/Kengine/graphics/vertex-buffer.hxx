#include "Kengine/graphics/raw-vertex-buffer.hxx"

namespace Kengine::graphics
{
    template <typename T>
    class vertex_buffer : public raw_vertex_buffer
    {
    public:
        void allocate_vertices(T*     vertices,
                               size_t count,
                               bool   dynamic) // make sure do bind() before
        {
            raw_vertex_buffer::allocate_data(
                vertices, static_cast<uint32_t>(count * sizeof(T)), dynamic);
        }

        void set_vertices(T*     vertices,
                          size_t index,
                          size_t count) // make sure do bind() before
        {
            raw_vertex_buffer::replace_data(
                vertices,
                static_cast<uint32_t>(index * sizeof(T)),
                static_cast<uint32_t>(count * sizeof(T)));
        }
    };
} // namespace Kengine::graphics
