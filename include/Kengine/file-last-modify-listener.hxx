#pragma once

#include <string>

#include "Kengine/event/user-events.hxx"

namespace Kengine
{
#ifdef ENGINE_DEV
using file_modify_callback = void (*)(void* data);

class file_last_modify_listener
{
public:
    virtual long add_file(std::string          file_path,
                          file_modify_callback f_modify_func,
                          void*                data) = 0;

    virtual void remove_file(long id) = 0;

    virtual void start_files_watch() = 0;

    virtual void handle_file_modify_listeners() = 0;

    static file_last_modify_listener* get_instance();
};
#endif
}; // namespace Kengine
