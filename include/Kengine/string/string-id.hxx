#pragma once

#include "Kengine/serialization/serialization.hxx"

#include <cstdint>
#include <functional>

namespace Kengine
{
    struct E_DECLSPEC string_id : public serializable
    {
        string_id();
        explicit string_id(const char* str);

        string_id& operator=(uint32_t val);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        const char* get_string() const;

        inline uint32_t get_id() const { return id; }

        inline bool empty() const { return !id; }

        operator uint32_t() const { return id; }

    private:
        void get_id_from_str(const char* c_str);

        uint32_t id = 0;
    };
} // namespace Kengine

template <>
struct std::hash<Kengine::string_id>
{
    std::size_t operator()(const Kengine::string_id& s_id) const noexcept
    {
        return static_cast<std::size_t>(uint32_t(s_id));
    }
};

namespace Kengine
{
    E_DECLSPEC string_id   hash_string(const char*);
    E_DECLSPEC const char* get_string(string_id);

    E_DECLSPEC string_id operator""_sid(const char* str);
} // namespace Kengine