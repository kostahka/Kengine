#pragma once

#include <cstdint>

namespace Kengine::graphics
{
    class E_DECLSPEC element_buffer
    {
    public:
        element_buffer();
        ~element_buffer();

        void allocate_indexes(uint32_t* indexes,
                              uint32_t  count,
                              bool      dynamic); // make sure do bind() before
        void set_indexes(uint32_t* indexes,
                         uint32_t  offset,
                         uint32_t  count); // make sure do bind() before

        void bind();

        inline uint32_t get_indexes_count() const { return indexes_count; };

        inline bool is_valid() const { return valid; };

    private:
        uint32_t ebo;
        uint32_t indexes_count;
        bool     valid;
    };
} // namespace Kengine::graphics