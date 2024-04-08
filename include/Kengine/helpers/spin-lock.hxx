#pragma once

#include <atomic>

namespace Kengine
{
    struct E_DECLSPEC spin_lock
    {
        void lock();
        void unlock();

    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    };
} // namespace Kengine