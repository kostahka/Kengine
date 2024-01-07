#pragma once

namespace Kengine
{
    typedef void* lib_handle;
    typedef void (*function_ptr)(void);

    lib_handle   load_lib(const char* path);
    void         unload_lib(lib_handle handle);
    function_ptr load_function(lib_handle handle, const char* func_name);

} // namespace Kengine