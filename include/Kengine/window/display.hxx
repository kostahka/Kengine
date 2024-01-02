#pragma once

namespace Kengine::display
{
    struct mode
    {
        int   w;
        int   h;
        float refresh_rate;

        bool operator==(const mode& other) const;
    };

    mode get_max_mode();

} // namespace Kengine::display