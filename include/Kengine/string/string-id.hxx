#pragma once

#include <cstdint>

namespace Kengine
{
    typedef uint32_t string_id;

    string_id   save_string(const char*);
    const char* get_string(string_id);
} // namespace Kengine