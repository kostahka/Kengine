#pragma once

#include <algorithm>
#include <cstdint>
#include <filesystem>
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
        virtual auto serialize_size() const -> std::size_t            = 0;
    };

    namespace serialization
    {
        template <typename T>
        concept Serializable = std::is_base_of_v<serializable, T>;

        constexpr char true_char  = 'T';
        constexpr char false_char = 'F';

        template <typename T>
        concept Boolean = std::is_same_v<T, bool>;

        template <typename T>
        concept String = std::is_same_v<T, std::string>;

        template <typename T>
        concept Path = std::is_same_v<T, std::filesystem::path>;

        template <typename T>
        concept Pair = !String<T> && !Path<T> && requires(T a) {
            typename T::first_type;
            typename T::second_type;
        };

        template <typename T>
        concept Container = !String<T> && !Path<T> && requires(T a) {
            typename T::value_type;
            typename T::reference;
            typename T::const_reference;
            typename T::iterator;
            typename T::const_iterator;
            typename T::size_type;
            {
                a.begin()
            } -> std::same_as<typename T::iterator>;
            {
                a.end()
            } -> std::same_as<typename T::iterator>;
            {
                a.cbegin()
            } -> std::same_as<typename T::const_iterator>;
            {
                a.cend()
            } -> std::same_as<typename T::const_iterator>;
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
            requires String<T>
        class stream_writer<T, void>
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
            requires Path<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                std::string string_path = value.string();
                return stream_writer<std::string>::write(os, string_path);
            };
        };

        template <typename T>
            requires Boolean<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos         = os.tellp();
                const auto write_value = value ? true_char : false_char;
                os.write(&write_value, sizeof(write_value));
                return static_cast<std::size_t>(os.tellp() - pos);
            };
        };

        template <typename T>
            requires Pair<T>
        class stream_writer<T, void>
        {
        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                using first_type  = typename T::first_type;
                using second_type = typename T::second_type;
                return stream_writer<first_type>::write(os, value.first) +
                       stream_writer<second_type>::write(os, value.second);
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
                const auto len = static_cast<typename T::size_type>(
                    std::distance(value.cbegin(), value.cend()));
                os.write(reinterpret_cast<const char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(os.tellp() - pos);
                if (len > 0)
                {
                    using value_t = typename T::value_type;
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
            template <typename I, typename V = void>
            struct converter
            {
                using type = I;
            };

            template <typename I>
                requires Pair<I>
            struct converter<I, void>
            {
                using type =
                    std::pair<std::remove_const_t<typename I::first_type>,
                              typename I::second_type>;
            };

        public:
            static auto write(std::ostream& os, const T& value) -> std::size_t
            {
                const auto pos = os.tellp();
                const auto len =
                    static_cast<typename T::size_type>(value.size());
                const auto len_size = sizeof(len);
                os.write(reinterpret_cast<const char*>(&len), len_size);
                auto size = static_cast<std::size_t>(os.tellp() - pos);
                if (len > 0)
                {
                    using value_t =
                        typename converter<typename T::value_type>::type;
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
                value.assign(len + 1, 0);
                if (len > 0)
                    is.read(value.data(), len);

                return static_cast<std::size_t>(is.tellg() - pos);
            };
        };

        template <typename T>
            requires Path<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                std::string string_path;
                auto size = stream_reader<std::string>::read(is, string_path);
                value     = std::filesystem::path(string_path);
                return size;
            };
        };

        template <typename T>
            requires Boolean<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto pos       = is.tellg();
                auto       read_char = false_char;
                is.read(reinterpret_cast<char*>(&read_char), sizeof(read_char));
                value = read_char == true_char;
                return static_cast<std::size_t>(is.tellg() - pos);
            };
        };

        template <typename T>
            requires Pair<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                using first_type  = typename T::first_type;
                using second_type = typename T::second_type;
                first_type  first_value{};
                second_type second_value{};
                auto size = stream_reader<first_type>::read(is, first_value) +
                            stream_reader<second_type>::read(is, second_value);
                T temp_value{ first_value, second_value };
                value = temp_value;
                return size;
            };
        };

        template <typename T>
            requires SequenceContainer<T>
        class stream_reader<T, void>
        {
        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto            pos      = is.tellg();
                typename T::size_type len      = 0;
                auto                  len_size = sizeof(len);
                is.read(reinterpret_cast<char*>(&len), len_size);
                auto size = static_cast<std::size_t>(is.tellg() - pos);
                if (len > 0)
                {
                    for (auto i = 0U; i < len; ++i)
                    {
                        using value_t = typename T::value_type;
                        value_t v{};
                        size += stream_reader<value_t>::read(is, v);
                        value.push_back(std::move(v));
                    }
                }
                return size;
            };
        };

        template <typename T>
            requires AssociativeContainer<T>
        class stream_reader<T, void>
        {
            template <typename I, typename V = void>
            struct converter
            {
                using type = I;
            };

            template <typename I>
                requires Pair<I>
            struct converter<I, void>
            {
                using type =
                    std::pair<std::remove_const_t<typename I::first_type>,
                              typename I::second_type>;
            };

        public:
            static auto read(std::istream& is, T& value) -> std::size_t
            {
                const auto            pos = is.tellg();
                typename T::size_type len = 0;
                is.read(reinterpret_cast<char*>(&len), sizeof(len));
                auto size = static_cast<std::size_t>(is.tellg() - pos);
                if (len > 0)
                {
                    for (auto i = 0U; i < len; ++i)
                    {
                        using value_t =
                            typename converter<typename T::value_type>::type;
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

        template <typename T, typename U = void>
        class stream_size
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                return sizeof(value);
            };
        };

        template <typename T>
            requires Serializable<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                return value.serialize_size();
            };
        };

        template <typename T>
            requires String<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                const auto len = static_cast<std::uint32_t>(value.size());
                return sizeof(len) + len;
            };
        };

        template <typename T>
            requires Path<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                std::string string_path = value.string();
                return stream_size<std::string>::size(string_path);
            };
        };

        template <typename T>
            requires Boolean<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                return sizeof(true_char);
            };
        };

        template <typename T>
            requires Pair<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                using first_type  = typename T::first_type;
                using second_type = typename T::second_type;
                return stream_size<first_type>::size(value.first) +
                       stream_size<second_type>::size(value.second);
            };
        };

        template <typename T>
            requires SequenceContainer<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                const auto len = static_cast<typename T::size_type>(
                    std::distance(value.cbegin(), value.cend()));
                auto size = sizeof(len);
                if (len > 0)
                {
                    using value_t = typename T::value_type;
                    std::for_each(value.cbegin(),
                                  value.cend(),
                                  [&](const auto& item) {
                                      size += stream_size<value_t>::size(item);
                                  });
                }
                return size;
            };
        };

        template <typename T>
            requires AssociativeContainer<T>
        class stream_size<T, void>
        {
        public:
            static auto size(const T& value) -> std::size_t
            {
                const auto len =
                    static_cast<typename T::size_type>(value.size());
                auto size = sizeof(len);
                if (len > 0)
                {
                    using value_t = typename T::value_type;
                    std::for_each(value.cbegin(),
                                  value.cend(),
                                  [&](const auto& item) {
                                      size += stream_size<value_t>::size(item);
                                  });
                }
                return size;
            };
        };

        template <typename T>
        auto size(const T& value) -> std::size_t
        {
            return stream_size<T>::size(value);
        };

    } // namespace serialization
} // namespace Kengine