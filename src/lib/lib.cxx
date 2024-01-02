#include "Kengine/lib/lib.hxx"

#include "Kengine/log/log.hxx"

#include <SDL3/SDL_loadso.h>

namespace Kengine
{
    lib_handle load_lib(const char* path)
    {
        lib_handle handle = SDL_LoadObject(path);
        if (!handle)
        {
            KENGINE_ERROR("Failed to load lib from [{}]. Error: {}",
                          path,
                          SDL_GetError());
        }
        return handle;
    }

    void unload_lib(lib_handle handle)
    {
        SDL_UnloadObject(handle);
    }

    function_ptr load_function(lib_handle handle, const char* func_name)
    {
        SDL_FunctionPointer f_ptr = SDL_LoadFunction(handle, func_name);
        if (!f_ptr)
        {
            KENGINE_ERROR("Failed to load function {} from lib. Error: {}",
                          func_name,
                          SDL_GetError());
        }
        return f_ptr;
    }
} // namespace Kengine