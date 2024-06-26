#pragma once

#include "Kengine/helpers/spin-lock.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "resource.hxx"

#include <cstddef>

namespace Kengine::resource_manager
{
    E_DECLSPEC res_ptr<resource> get_resource(string_id r_id);
    E_DECLSPEC res_ptr<resource> load_resource(path res_path);
    E_DECLSPEC void              remove_resource(string_id r_id);

    void initialize();
    void shutdown();
} // namespace Kengine::resource_manager

namespace Kengine
{
    template <class ResourceType>
    class res_ptr;
    template <class ResourceType>
    class res_weak_ptr;

    template <class ToResourceType, class FromResourceType>
    res_ptr<ToResourceType> static_resource_cast(
        const res_ptr<FromResourceType>& ptr)
    {
        return res_ptr<ToResourceType>(ptr.counter);
    }

    template <class ToResourceType, class FromResourceType>
    res_weak_ptr<ToResourceType> static_resource_cast(
        const res_weak_ptr<FromResourceType>& ptr)
    {
        return res_weak_ptr<ToResourceType>(ptr.counter);
    }

    class res_ptr_counter
    {
        template <class ResourceType>
        friend class res_ptr;
        template <class ResourceType>
        friend class res_weak_ptr;

    public:
        res_ptr_counter(resource* res)
            : p_count(0)
            , w_count(0)
            , ptr(res)
        {
        }

        res_ptr_counter(resource* res, path r_path)
            : p_count(0)
            , w_count(0)
            , ptr(res)
            , r_path(r_path)
        {
        }

        void free()
        {
            if (ptr)
            {
                delete ptr;
                ptr = nullptr;
            }
        }

        ~res_ptr_counter()
        {
            if (ptr)
                delete ptr;
        }

    private:
        size_t    p_count;
        size_t    w_count;
        resource* ptr;
        path      r_path;
    };

    template <class ResourceType>
    class res_weak_ptr
    {
        template <class ToResourceType, class FromResourceType>
        friend res_weak_ptr<ToResourceType> static_resource_cast(
            const res_weak_ptr<FromResourceType>& ptr);

        friend void resource_manager::shutdown();
        friend void resource_manager::initialize();

    public:
        res_weak_ptr()
            : counter(nullptr)
        {
        }

        constexpr res_weak_ptr(std::nullptr_t)
            : counter(nullptr)
        {
        }

        res_weak_ptr(const res_ptr<ResourceType>& ptr)
        {
            counter = ptr.counter;
            if (counter)
            {
                counter->w_count++;
            }
        }

        explicit res_weak_ptr(res_ptr_counter* counter)
            : counter(counter)
        {
            if (counter)
            {
                counter->w_count++;
            }
        }

        res_weak_ptr(const res_weak_ptr& other)
            : counter(other.counter)
        {
            if (counter)
            {
                counter->w_count++;
            }
        }

        res_weak_ptr(res_weak_ptr&& other)
        {
            counter       = other.counter;
            other.counter = nullptr;
        }

        res_weak_ptr& operator=(const res_weak_ptr& other)
        {
            free_counter();
            counter = other.counter;
            if (counter)
            {
                counter->w_count++;
            }

            return *this;
        }

        res_weak_ptr& operator=(res_weak_ptr&& other)
        {
            free_counter();
            counter       = other.counter;
            other.counter = nullptr;

            return *this;
        }

        ~res_weak_ptr() { free_counter(); }

        res_ptr<ResourceType> lock()
        {
            if (counter && counter->ptr != nullptr)
                return res_ptr<ResourceType>(counter);
            else
                return res_ptr<ResourceType>(nullptr);
        }

    private:
        void free_counter()
        {
            if (counter)
            {
                counter->w_count--;
                if (counter->p_count <= 0 && counter->w_count <= 0)
                {
                    delete counter;
                }
            }
        }

        res_ptr_counter* counter;
    };

    template <class ResourceType>
    class res_ptr : public serializable
    {
        static_assert(std::is_base_of_v<resource, ResourceType>,
                      "ResourceType only inhertis resource");

        friend class res_weak_ptr<ResourceType>;

        template <class ToResourceType, class FromResourceType>
        friend res_ptr<ToResourceType> static_resource_cast(
            const res_ptr<FromResourceType>& ptr);

        friend void resource_manager::shutdown();
        friend void resource_manager::initialize();

    public:
        using element_type = ResourceType;

        res_ptr()
            : ptr(nullptr)
            , counter(nullptr)
        {
        }

        constexpr res_ptr(std::nullptr_t)
            : ptr(nullptr)
            , counter(nullptr)
        {
        }

        explicit res_ptr(ResourceType* res)
            : ptr(res)

        {
            counter = new res_ptr_counter(static_cast<resource*>(res));
            counter->p_count++;
        }

        explicit res_ptr(ResourceType* res, path r_path)
            : ptr(res)

        {
            counter = new res_ptr_counter(static_cast<resource*>(res), r_path);
            counter->p_count++;
        }

        explicit res_ptr(res_ptr_counter* counter)
            : counter(counter)
            , ptr(nullptr)
        {
            if (counter)
            {
                ptr = static_cast<ResourceType*>(counter->ptr);

                counter->p_count++;
            }
        }

        res_ptr(const res_ptr& other)
        {
            ptr     = other.ptr;
            counter = other.counter;
            if (counter)
            {
                counter->p_count++;
            }
        }

        res_ptr(res_ptr&& other)
        {
            ptr           = other.ptr;
            counter       = other.counter;
            other.ptr     = nullptr;
            other.counter = nullptr;
        }

        operator res_ptr<resource>()
        {
            return res_ptr<resource>(this->counter);
        }

        res_ptr& operator=(const res_ptr& other)
        {
            free_counter();
            ptr     = other.ptr;
            counter = other.counter;
            if (counter)
            {
                counter->p_count++;
            }

            return *this;
        }

        res_ptr& operator=(res_ptr&& other)
        {
            free_counter();
            ptr           = other.ptr;
            counter       = other.counter;
            other.ptr     = nullptr;
            other.counter = nullptr;

            return *this;
        }

        inline operator bool() const { return ptr != nullptr; }

        ~res_ptr() { free_counter(); }

        std::size_t serialize(std::ostream& os) const override
        {
            path r_path;
            if (counter)
            {
                r_path = counter->r_path;
            }
            std::size_t size = 0;
            size += serialization::write(os, r_path.string());

            if (ptr)
                size += serialization::write(os, ptr->get_resource_id());
            else
                size += serialization::write(os, string_id());

            return size;
        }

        std::size_t deserialize(std::istream& is) override
        {
            res_ptr<resource> res = nullptr;
            string_id         res_id{};

            std::string r_path_str;

            std::size_t size = 0;

            size += serialization::read(is, r_path_str);
            size += serialization::read(is, res_id);

            path r_path = r_path_str;
            if (res_id)
            {
                res = resource_manager::get_resource(res_id);
                if (!res)
                {
                    res = resource_manager::load_resource(r_path);
                }
                *this = static_resource_cast<ResourceType>(res);
            }
            return size;
        }

        std::size_t serialize_size() const override
        {
            path r_path;
            if (counter)
            {
                r_path = counter->r_path;
            }
            std::size_t size = 0;
            size += serialization::size(r_path.string());

            if (ptr)
                size += serialization::size(ptr->get_resource_id());
            else
                size += serialization::size(string_id());

            return size;
        }

        [[nodiscard]] inline ResourceType* get() const { return ptr; }

        inline ResourceType& operator*() { return *ptr; }

        inline const ResourceType& operator*() const { return *ptr; }

        inline ResourceType* operator->() { return ptr; }

        inline const ResourceType* operator->() const { return ptr; }

        [[nodiscard]] inline path get_res_path()
        {
            if (counter)
            {
                return counter->r_path;
            }

            return {};
        };

    private:
        void free_counter()
        {
            if (counter)
            {
                counter->p_count--;

                if (counter->p_count <= 0)
                {
                    if (counter->w_count <= 0)
                    {
                        delete counter;
                    }
                    else
                    {
                        auto r_id = ptr->get_resource_id();
                        counter->free();
                        resource_manager::remove_resource(r_id);
                    }
                }
            }
        }

        ResourceType*    ptr;
        res_ptr_counter* counter;
    };
} // namespace Kengine