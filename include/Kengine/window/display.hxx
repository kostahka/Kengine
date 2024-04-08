#pragma once

namespace Kengine::display
{
    struct E_DECLSPEC mode
    {
        int   w;
        int   h;
        float refresh_rate;

        bool operator==(const mode& other) const;
        bool operator!=(const mode& other) const;
    };

    E_DECLSPEC mode get_max_mode();

} // namespace Kengine::display