#pragma once

#include "Kengine/io/file-manager.hxx"

#include <SDL3/SDL_iostream.h>

namespace Kengine::file_manager
{
    struct file_buffer_impl;

    class sdl_io_stream
    {
    public:
        sdl_io_stream(std::unique_ptr<file_buffer>& file);
        ~sdl_io_stream();

        static Sint64 size(void* userdata);
        static Sint64 seek(void* userdata, Sint64 offset, int whence);

        static size_t read(void*         userdata,
                           void*         ptr,
                           size_t        size,
                           SDL_IOStatus* status);

        static size_t write(void*         userdata,
                            const void*   ptr,
                            size_t        size,
                            SDL_IOStatus* status);

        static int close(void* userdata);

        inline SDL_IOStream* get() const { return stream; }

    private:
        std::iostream                     io;
        std::unique_ptr<file_buffer_impl> file = nullptr;
        SDL_IOStream*                     stream;
        SDL_IOStreamInterface             interface;
    };
} // namespace Kengine::file_manager