#pragma once

#include "Kengine/io/file-manager.hxx"

#include <SDL3/SDL_iostream.h>

namespace Kengine::file_manager
{
    struct file_buffer_impl : public file_buffer
    {
        file_buffer_impl();

        bool open(std::filesystem::path   path,
                  std::ios_base::openmode mode,
                  size_t                  abuf_size) override;

        bool load(std::filesystem::path path);

        void close() override;

        ~file_buffer_impl() override;

        int total_size();

    protected:
        int sync() override;

        int_type overflow(int_type c) override;

        int_type underflow() override;

        pos_type seekoff(off_type                off,
                         std::ios_base::seekdir  dir,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override;

        pos_type seekpos(pos_type                pos,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override;

    private:
        SDL_IOStream* file;
        size_t        file_write_pos;
        size_t        file_read_pos;
        size_t        file_curr_pos;
    };

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