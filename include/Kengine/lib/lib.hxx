#pragma once

namespace Kengine
{
    typedef void* lib_handle;
    typedef void (*function_ptr)(void);

    E_DECLSPEC lib_handle   load_lib(const char* path);
    E_DECLSPEC void         unload_lib(lib_handle handle);
    E_DECLSPEC function_ptr load_function(lib_handle  handle,
                                          const char* func_name);

} // namespace Kengine