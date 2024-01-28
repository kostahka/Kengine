#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

namespace Kengine
{
    class serializable
    {
    protected:
        ~serializable() = default;

    public:
        virtual auto serialize(std::ostream& os) const -> std::size_t = 0;
        virtual auto deserialize(std::istream& is) -> std::size_t     = 0;
    };

    namespace serialization
    {
        template <typename T>
        using only_if_string = std::enable_if_t<std::is_same_v<T, std::string>>;

        template <typename T>
        concept Serializable = std::is_base_of_v<serializable, T>;

        template <typename T>
        concept String = std::is_same_v<T, std::string>;

        template <typename T>
        concept Container = !String<T> && requires(T a) {
            typename T::value_type;
            typename T::reference;
            typename T::const_reference;
            typename T::iterator;
            typename T::const_iterator;
            typename T::size_type;
            {
                a.begin()
            } -> std::same_as<T::iterator>;
            {
                a.end()
            } -> std::same_as<T::iterator>;
            {
                a.cbegin()
            } -> std::same_as<T::const_iterator>;
            {
                a.cend()
            } -> std::same_as<T::const_iterator>;
            {
                a.clear()
            } -> std::same_as<void>;
        };

        template <typename T>
        concept SequenceContainer =
            Container<T> && requires(T a, typename T::size_type count) {
                {
                    a.resize(count)
                } -> std::same_as<void>;
            };

        template <typename T>
        concept AssociativeContainer =
            Container<T> && requires(T a) { typename T::key_type; };

        template <typename T, typename U = void>
        class stream_writer
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos = os.tellp();
                os.write(reinterpret_cast<const char*>(&value), sizeof(value));
                return static_cast<std::size_t>(os.tellp() - pos);
            };
        };

        template <typename T>
            requires Serializable<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                return value.serialize(os);
            };
        };

        template <typename T>
        class stream_writer<T, only_if_string<T>>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos = os.tellp();
                const auto len = static_cast<std::uint32_t>(value.size());
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                if (len > 0)
                    os.write(value.data(), len);
                return static_cast<std::size_t>(os.tellp() - pos);
            };
        };

        template <typename T>
            requires SequenceContainer<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos = os.tellp();
                // to support std::forward_list we have to use std::distance()
                const auto len = static_cast<std::uint16_t>(
                    std::distance(value.cbegin(), value.cend()));
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(os.tellp() - pos);
                if (len > 0)
                {
                    using value_t = typename stream_writer::value_type;
                    std::for_each(
                        value.cbegin(),
                        value.cend(),
                        [&](const auto& item)
                        { size += stream_writer<value_t>::write(os, item); });
                }
                return size;
            };
        };

        template <typename T>
            requires AssociativeContainer<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos = os.tellp();
                const auto len = static_cast<typename stream_writer::size_type>(
                    value.size());
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(os.tellp() - pos);
                if (len > 0)
                {
                    using value_t = typename stream_writer::value_type;
                    std::for_each(
                        value.cbegin(),
                        value.cend(),
                        [&](const auto& item)
                        { size += stream_writer<value_t>::write(os, item); });
                }
                return size;
            };
        };

        template <typename T>
        auto write(std::ostream& os, const T& value) -> std::size_t
        {
            return stream_writer<T>::write(os, value);
        };

        template <typename T, typename U = void>
        class stream_reader
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto pos = is.tellg();
                is.read(reinterpret_cast<char*>(&value), sizeof(value));
                return static_cast<std::size_t>(is.tellg() - pos);
            };
        };

        template <typename T>
            requires Serializable<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                return value.deserialize(is);
            };
        };

        template <typename T>
            requires String<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto    pos = is.tellg();
                std::uint32_t len = 0;
                is.read(reinterpret_cast<char*>(&len), sizeof(len));
                value.resize(len);
                if (len > 0)
                    is.read(value.data(), len);

                return static_cast<std::size_t>(is.tellg() - pos);
            };
        };

        template <typename T>
            requires SequenceContainer<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto                        pos = is.tellg();
                typename stream_reader::size_type len = 0;
                is.read(reinterpret_cast<char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(is.tellg() - pos);
                if (len > 0)
                {
                    for (auto i = 0U; i < len; ++i)
                    {
                        using value_t = typename stream_reader::value_type;
                        value_t v{};
                        size += stream_reader<value_t>::read(is, v);
                        value.insert(std::move(v));
                    }
                }
                return size;
            };
        };

        template <typename T>
            requires AssociativeContainer<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto                        pos = is.tellg();
                typename stream_reader::size_type len = 0;
                is.read(reinterpret_cast<char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(is.tellg() - pos);
                if (len > 0)
                {
                    for (auto i = 0U; i < len; ++i)
                    {
                        using value_t = typename stream_reader::value_type;
                        value_t v{};
                        size += stream_reader<value_t>::read(is, v);
                        value.insert(std::move(v));
                    }
                }
                return size;
            };
        };

        template <typename T>
        auto read(std::istream& is, T& value) -> std::size_t
        {
            return stream_reader<T>::read(is, value);
        };
    } // namespace serialization
} // namespace Kengine