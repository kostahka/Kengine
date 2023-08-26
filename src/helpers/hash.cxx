#include "Kengine/helpers/hash.hxx"

#include <array>

namespace Kengine::hash
{
    constexpr std::array<unsigned long, 256> crc_table{
        []() constexpr
        {
            std::array<unsigned long, 256> res{ 0 };
            unsigned long                  crc{ 0 };
            for (int i = 0; i < 256; i++)
            {
                crc = i;
                for (int j = 0; j < 8; j++)
                    crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
                res[i] = crc;
            };
            return res;
        }()
    };

    uint32_t crc32(const char* buf, unsigned long len)
    {
        unsigned long crc;
        crc = 0xFFFFFFFFUL;
        while (len--)
            crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
        return crc ^ 0xFFFFFFFFUL;
    }

    uint32_t crc32(const char* c_str)
    {
        unsigned long crc;
        crc = 0xFFFFFFFFUL;
        while (*c_str)
            crc = crc_table[(crc ^ *c_str++) & 0xFF] ^ (crc >> 8);
        return crc ^ 0xFFFFFFFFUL;
    }
} // namespace Kengine::hash
