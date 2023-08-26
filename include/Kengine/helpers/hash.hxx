#pragma once

#include <cstdint>

namespace Kengine::hash
{
    uint32_t crc32(const char* buf, unsigned long len);
    uint32_t crc32(const char* c_str);
} // namespace Kengine::hash
