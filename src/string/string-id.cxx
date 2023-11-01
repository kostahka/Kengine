#include "Kengine/string/string-id.hxx"

#include <unordered_map>

#include "Kengine/helpers/hash.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine
{
    std::unordered_map<string_id, const char*> string_table;

    string_id save_string(const char* c_str)
    {
        string_id id = hash::crc32(c_str);
        if (string_table.contains(id))
        {
            KENGINE_WARN("String table already has such key. Check if you "
                         "save one string "
                         "more than once, or try to edit string");
            return id;
        }

        size_t str_len  = strlen(c_str);
        char*  copy_str = new char[str_len + 1];
        strcpy_s(copy_str, str_len + 1, c_str);

        string_table[id] = copy_str;
        return id;
    }

    const char* get_string(string_id id)
    {
        auto s = string_table.find(id);
        KENGINE_ASSERT(s != string_table.end(), "No such string id in table.");
        return s != string_table.end() ? s->second : "<STRING NOT FOUND>";
    }

    string_id operator""_sid(const char* str)
    {
        return save_string(str);
    }
} // namespace Kengine