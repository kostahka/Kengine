#pragma once

#include "resource.hxx"
#include <cstddef>

namespace Kengine::resource_manager
{
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

        void free()
        {
            delete ptr;
            ptr = nullptr;
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
        res_weak_ptr(const res_ptr<ResourceType>& ptr)
        {
            counter = ptr.counter;
            if (counter)
                counter->w_count++;
        }

        res_weak_ptr()
            : counter(nullptr)
        {
        }

        res_weak_ptr(std::nullptr_t)
            : counter(nullptr)
        {
        }

        explicit res_weak_ptr(res_ptr_counter* counter)
            : counter(counter)
        {
            if (counter)
                counter->w_count++;
        }

        ~res_weak_ptr()
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

        res_ptr<ResourceType> lock()
        {
            if (counter && counter->ptr != nullptr)
                return res_ptr<ResourceType>(counter);
            else
                return res_ptr<ResourceType>(nullptr);
        }

    private:
        res_ptr_counter* counter;
    };

    template <class ResourceType>
    class res_ptr
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

        explicit res_ptr(res_ptr_counter* counter)
            : counter(counter)
        {
            ptr = static_cast<ResourceType*>(counter->ptr);
            if (counter)
                counter->p_count++;
        }

        res_ptr(const res_ptr& other)
        {
            ptr     = other.ptr;
            counter = other.counter;
            if (counter)
                counter->p_count++;
        }

        res_ptr(res_ptr&& other)
        {
            ptr           = other.ptr;
            counter       = other.counter;
            other.ptr     = nullptr;
            other.counter = nullptr;
        }

        res_ptr& operator=(const res_ptr& other)
        {
            ptr     = other.ptr;
            counter = other.counter;
            if (counter)
                counter->p_count++;

            return *this;
        }

        res_ptr& operator=(res_ptr&& other)
        {
            ptr           = other.ptr;
            counter       = other.counter;
            other.ptr     = nullptr;
            other.counter = nullptr;

            return *this;
        }

        inline operator bool() { return ptr != nullptr; }

        ~res_ptr()
        {
            if (counter)
            {
                counter->p_count--;
                if (counter->p_count <= 0)
                {
                    if (counter->w_count <= 0)
                        delete counter;
                    else
                        counter->free();
                }
            }
        }

        [[nodiscard]] inline ResourceType* get() const { return ptr; }

        inline ResourceType& operator*() { return *ptr; }

        inline const ResourceType& operator*() const { return *ptr; }

        inline ResourceType* operator->() { return ptr; }

        inline const ResourceType* operator->() const { return ptr; }

    private:
        ResourceType*    ptr;
        res_ptr_counter* counter;
    };
} // namespace Kengine