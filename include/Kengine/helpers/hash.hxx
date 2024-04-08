#pragma once

#include <cstdint>

namespace Kengine::hash
{
    E_DECLSPEC uint32_t crc32(const char* buf, unsigned long len);
    E_DECLSPEC uint32_t crc32(const char* c_str);
} // namespace Kengine::hash
