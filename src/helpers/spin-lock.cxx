#include "Kengine/helpers/spin-lock.hxx"

namespace Kengine
{
    void spin_lock::lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
        {
        }
    }

    void spin_lock::unlock()
    {
        flag.clear(std::memory_order_release);
    }
} // namespace Kengine