#include "display.hxx"

#include "Kengine/log/log.hxx"

#include "SDL3/SDL_video.h"

#include <vector>

namespace Kengine::display
{
    SDL_DisplayID          primary;
    std::vector<mode>      d_modes;
    mode                   desktop_d_mode;
    const SDL_DisplayMode* sdl_desktop_d_mode = nullptr;

    bool mode::operator==(const mode& other) const
    {
        return w == other.w && h == other.h &&
               refresh_rate == other.refresh_rate;
    }

    bool mode::operator!=(const mode& other) const
    {
        return !(*this == other);
    }

    bool initialize()
    {
        d_modes.clear();

        primary = SDL_GetPrimaryDisplay();
        if (!primary)
        {
            KENGINE_FATAL("Failed to get primary display. Error: {}",
                          SDL_GetError());
            return false;
        }

        {
            sdl_desktop_d_mode = SDL_GetDesktopDisplayMode(primary);

            desktop_d_mode = { .w = sdl_desktop_d_mode->h,
                               .h = sdl_desktop_d_mode->w,
                               .refresh_rate =
                                   sdl_desktop_d_mode->refresh_rate };
        }

        {
            int                           d_modes_count;
            const SDL_DisplayMode* const* modes =
                SDL_GetFullscreenDisplayModes(primary, &d_modes_count);
            if (!modes)
            {
                KENGINE_FATAL(
                    "Failed to get display fullscreen modes. Error: {}",
                    SDL_GetError());
                return false;
            }
            if (d_modes_count > 0)
            {
                mode last_mode = { 0, 0, 0 };
                for (int i = 0; i < d_modes_count; ++i)
                {
                    mode d_mode{ modes[i]->w,
                                 modes[i]->h,
                                 modes[i]->refresh_rate };

                    if (d_mode != last_mode)
                    {
                        last_mode = d_mode;
                        d_modes.push_back(d_mode);
                    }
                }
            }
        }

        return true;
    }

    mode get_max_mode()
    {
        if (d_modes.size() > 0)
            return d_modes.front();
        else
            return desktop_d_mode;
    }

    bool get_closest_display_mode(mode d_mode, SDL_DisplayMode* sdl_mode)
    {
        if (d_modes.size() > 0)
        {
            auto success =
                SDL_GetClosestFullscreenDisplayMode(primary,
                                                    d_mode.w,
                                                    d_mode.h,
                                                    d_mode.refresh_rate,
                                                    true,
                                                    sdl_mode);
            if (!success && !d_modes.empty())
            {
                success =
                    SDL_GetClosestFullscreenDisplayMode(primary,
                                                        d_modes[0].w,
                                                        d_modes[0].h,
                                                        d_modes[0].refresh_rate,
                                                        true,
                                                        sdl_mode);
            }
            return success;
        }

        return false;
    }
} // namespace Kengine::display