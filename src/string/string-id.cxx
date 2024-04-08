#include "Kengine/string/string-id.hxx"

#include "Kengine/helpers/hash.hxx"
#include "Kengine/log/log.hxx"

#include <string.h>
#include <unordered_map>

namespace Kengine
{
    std::unordered_map<uint32_t, const char*> string_table;

    string_id::string_id()
        : id(0)
    {
    }

    string_id::string_id(const char* str)
    {
        get_id_from_str(str);
    }

    string_id& string_id::operator=(uint32_t val)
    {
        id = val;
        return *this;
    }

    std::size_t string_id::serialize(std::ostream& os) const
    {
        std::string str = id ? get_string() : "";
        return serialization::write(os, str);
    }

    std::size_t string_id::deserialize(std::istream& is)
    {
        std::string str;
        auto        size = serialization::read(is, str);

        get_id_from_str(str.c_str());

        return size;
    }

    std::size_t string_id::serialize_size() const
    {
        std::string str = id ? get_string() : "";
        return serialization::size(str);
    }

    const char* string_id::get_string() const
    {
        auto s = string_table.find(id);
        KENGINE_ASSERT_WARN(s != string_table.end(),
                            "No such string id in table.");
        return s != string_table.end() ? s->second : "<STRING NOT FOUND>";
    }

    void string_id::get_id_from_str(const char* c_str)
    {
        size_t str_len = strlen(c_str);

        if (strlen == 0)
        {
            id = 0;
            return;
        }

        id = hash::crc32(c_str);

        KENGINE_ASSERT_WARN(id, "Zero string id");
        if (id != 0)
        {
            char* copy_str = new char[str_len + 1];
            strcpy(copy_str, c_str);

            string_table[id] = copy_str;
        }
    }

    string_id hash_string(const char* c_str)
    {
        return string_id(c_str);
    }

    const char* get_string(string_id id)
    {
        return id.get_string();
    }

    string_id operator""_sid(const char* str)
    {
        return string_id(str);
    }
} // namespace Kengine