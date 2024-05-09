#include "file-manager.hxx"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>

#include "Kengine/log/log.hxx"

#include <cstdlib>
#include <unordered_map>

namespace Kengine::file_manager
{
    std::unordered_map<std::ios_base::openmode, const char*> openmodes{
        {std::ios_base::in,                                                "r" },
        { std::ios_base::in | std::ios_base::binary,                       "rb"},
        { std::ios_base::in | std::ios_base::out,                          "r+"},
        { std::ios_base::in | std::ios_base::out | std::ios_base::binary,
         "r+b"                                                                 },

        { std::ios_base::out,                                              "w" },
        { std::ios_base::out | std::ios_base::trunc,                       "w" },
        { std::ios_base::out | std::ios_base::binary,                      "wb"},
        { std::ios_base::in | std::ios_base::out | std::ios_base::trunc,   "w+"},
        { std::ios_base::in | std::ios_base::out | std::ios_base::trunc |
              std::ios_base::binary,
         "w+b"                                                                 },

        { std::ios_base::app,                                              "a" },
        { std::ios_base::app | std::ios_base::out,                         "a" },
        { std::ios_base::app | std::ios_base::binary,                      "ab"},
        { std::ios_base::app | std::ios_base::binary | std::ios_base::out,
         "ab"                                                                  },
        { std::ios_base::app | std::ios_base::in,                          "a+"},
        { std::ios_base::app | std::ios_base::in | std::ios_base::out,     "a+"},
        { std::ios_base::app | std::ios_base::in | std::ios_base::binary,
         "a+b"                                                                 },
        { std::ios_base::app | std::ios_base::in | std::ios_base::out |
              std::ios_base::binary,
         "a+b"                                                                 }
    };

    std::unordered_map<std::ios_base::seekdir, int> seekdirs{
        {std::ios_base::beg,  SDL_IO_SEEK_SET},
        { std::ios_base::end, SDL_IO_SEEK_END},
        { std::ios_base::cur, SDL_IO_SEEK_CUR}
    };

    std::unordered_map<int, std::ios_base::seekdir> sdl_seekdirs{
        {SDL_IO_SEEK_SET,  std::ios_base::beg},
        { SDL_IO_SEEK_END, std::ios_base::end},
        { SDL_IO_SEEK_CUR, std::ios_base::cur}
    };

    file_buffer::file_buffer()
        : is_file_open(false)
        , read_buffer(nullptr)
        , write_buffer(nullptr)
        , buf_size(0)
        , file_size(0)
    {
    }

    file_buffer::~file_buffer() {}

    file_buffer_impl::file_buffer_impl()
        : file_buffer()
        , file(nullptr)
        , file_write_pos(0)
        , file_read_pos(0)
        , file_curr_pos(0)
    {
        setg(nullptr, nullptr, nullptr);
        setp(nullptr, nullptr);
    }

    bool file_buffer_impl::open(std::filesystem::path   path,
                                std::ios_base::openmode mode,
                                size_t                  abuf_size)
    {
        auto sdl_openmode_map = openmodes.find(mode);
        if (sdl_openmode_map == openmodes.end())
        {
            KENGINE_ERROR("Undefined openmode");
            return false;
        }

        file = SDL_IOFromFile(path.string().c_str(), sdl_openmode_map->second);
        if (!file)
        {
            KENGINE_ERROR("Failed to open file [{}], error: {}",
                          path.string().c_str(),
                          SDL_GetError());
            return false;
        }
        is_file_open = true;
        if (abuf_size)
        {
            buf_size = abuf_size;
        }
        else
        {
            buf_size = 10;
        }
        file_size      = SDL_GetIOSize(file);
        file_curr_pos  = mode & std::ios_base::app ? file_size : 0;
        file_write_pos = file_read_pos = file_curr_pos;

        if (mode & std::ios_base::in)
        {
            read_buffer = std::make_unique<char[]>(buf_size);
            setg(read_buffer.get(),
                 read_buffer.get() + buf_size,
                 read_buffer.get() + buf_size);
        }

        if (mode & std::ios_base::out || mode & std::ios_base::app)
        {
            write_buffer = std::make_unique<char[]>(buf_size);
            setp(write_buffer.get(), write_buffer.get() + buf_size);
        }

        if (sync() != 0)
        {
            close();
            return false;
        }

        KENGINE_INFO("Loaded file: {}", path.string().c_str());

        return true;
    }

    bool file_buffer_impl::load(std::filesystem::path path)
    {
        file = SDL_IOFromFile(path.string().c_str(), "rb");
        if (!file)
        {
            KENGINE_ERROR("Failed to open file [{}], error: {}",
                          path.string().c_str(),
                          SDL_GetError());
            return false;
        }

        is_file_open   = true;
        file_size      = SDL_GetIOSize(file);
        file_curr_pos  = 0;
        file_write_pos = 0;
        file_read_pos  = 0;

        if (file_size)
            buf_size = file_size;
        else
        {
            close();
            return false;
        }

        read_buffer = std::make_unique<char[]>(buf_size);
        setg(read_buffer.get(),
             read_buffer.get() + buf_size,
             read_buffer.get() + buf_size);

        if (sync() != 0)
        {
            close();
            return false;
        }

        KENGINE_INFO("Loaded file: {}", path.string().c_str());

        return true;
    }

    void file_buffer_impl::close()
    {
        if (file)
        {
            if (0 != SDL_CloseIO(file))
            {
                KENGINE_ERROR("Failed to close file.");
            }
            file = nullptr;
        }
        setg(nullptr, nullptr, nullptr);
        setp(nullptr, nullptr);
        read_buffer   = nullptr;
        write_buffer  = nullptr;
        buf_size      = 0;
        file_size     = 0;
        file_read_pos = file_write_pos = file_curr_pos = 0;

        is_file_open = false;
    }

    file_buffer_impl::~file_buffer_impl()
    {
        close();
    }

    int file_buffer_impl::total_size()
    {
        if (file)
        {
            return static_cast<int>(SDL_GetIOSize(file));
        }
        return -1;
    }

    int file_buffer_impl::sync()
    {
        if (!file)
            return -1;

        if (pptr() != pbase())
        {
            if (file_curr_pos != file_write_pos)
            {
                file_curr_pos =
                    SDL_SeekIO(file, file_write_pos, SDL_IO_SEEK_SET);
                if (file_curr_pos == -1)
                {
                    KENGINE_ERROR("Can't write to file.");
                    return -1;
                }
            }
            size_t must_write_bytes = pptr() - pbase();
            Sint64 write_bytes =
                SDL_WriteIO(file, write_buffer.get(), must_write_bytes);
            if (write_bytes != must_write_bytes)
            {
                KENGINE_ERROR("Can't write to file.");
                return -1;
            }
            pbump(-static_cast<int>(write_bytes));
            file_write_pos += write_bytes;
            file_curr_pos = file_write_pos;

            if (file_curr_pos > file_size)
                file_size = file_curr_pos;
        }
        if (gptr() != eback())
        {
            file_read_pos -= egptr() - gptr();
            if (file_curr_pos != file_read_pos)
            {
                file_curr_pos =
                    SDL_SeekIO(file, file_read_pos, SDL_IO_SEEK_SET);
                if (file_curr_pos == -1)
                {
                    KENGINE_ERROR("Can't read from file.");
                    return -1;
                }
            }
            Sint64 read_bytes = SDL_ReadIO(file, read_buffer.get(), buf_size);
            if (read_bytes == -1)
            {
                KENGINE_ERROR("Can't read from file.");
                return -1;
            }

            setg(read_buffer.get(),
                 read_buffer.get(),
                 read_buffer.get() + read_bytes);

            if (read_bytes < static_cast<Sint64>(buf_size))
            {
                read_buffer.get()[read_bytes] = traits_type::eof();
            }
            file_read_pos += read_bytes;
            file_curr_pos = file_read_pos;
        }

        return 0;
    }

    file_buffer_impl::int_type file_buffer_impl::overflow(int_type c)
    {
        if (!pptr())
            return traits_type::eof();

        if (c == traits_type::eof())
            return traits_type::eof();

        if (sync() != 0)
            return traits_type::eof();

        *pptr() = c;
        pbump(1);
        return c;
    }

    file_buffer_impl::int_type file_buffer_impl::underflow()
    {
        if (!gptr())
            return traits_type::eof();

        if (sync() != 0)
            return traits_type::eof();

        return *gptr();
    }

    file_buffer_impl::pos_type file_buffer_impl::seekoff(
        off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which)
    {
        if (sync() != 0)
            return -1;

        size_t new_read_pos  = 0;
        size_t new_write_pos = 0;

        switch (dir)
        {
            case std::ios_base::beg:
                new_read_pos  = off;
                new_write_pos = off;
                break;
            case std::ios_base::end:
                new_read_pos  = file_size + off;
                new_write_pos = file_size + off;
                break;
            case std::ios_base::cur:
                new_read_pos  = file_read_pos - (egptr() - gptr()) + off;
                new_write_pos = file_write_pos + off;
                break;
            default:
                return -1;
                break;
        }

        pos_type pos = 0;

        if (which & std::ios_base::in)
        {
            if (file_read_pos - new_read_pos <=
                    static_cast<size_t>(egptr() - eback()) &&
                new_read_pos < file_read_pos)
            {
                setg(
                    eback(), egptr() - (file_read_pos - new_read_pos), egptr());

                pos = new_read_pos;
            }
            else
            {
                pos = file_read_pos = new_read_pos;
                setg(eback(), egptr(), egptr());
            }
        }

        if (which & std::ios_base::out)
        {
            pos = file_write_pos = new_write_pos;
        }

        return pos;
    }

    file_buffer_impl::pos_type file_buffer_impl::seekpos(
        pos_type pos, std::ios_base::openmode which)
    {
        return seekoff(pos, std::ios_base::beg, which);
    }

    std::unique_ptr<file_buffer> file_buffer::get_empty()
    {
        return std::make_unique<file_buffer_impl>();
    }

    std::unique_ptr<file_buffer> open_file(std::filesystem::path   path,
                                           std::ios_base::openmode mode,
                                           size_t                  buf_size)
    {
        auto f_buffer = std::make_unique<file_buffer_impl>();
        if (f_buffer->open(path, mode, buf_size))
            return f_buffer;
        else
            return nullptr;
    }

    std::unique_ptr<file_buffer> load_file(std::filesystem::path path)
    {
        auto f_buffer = std::make_unique<file_buffer_impl>();
        if (f_buffer->load(path))
            return f_buffer;
        else
            return nullptr;
    }

    bool file_exists(std::filesystem::path path)
    {
        auto file = SDL_IOFromFile(path.string().c_str(), "rb");
        if (file)
        {
            SDL_CloseIO(file);
            return true;
        }
        else
            return false;
    }

    sdl_io_stream::sdl_io_stream(std::unique_ptr<file_buffer>& file)
        : io(file.get())
        , file(static_cast<file_buffer_impl*>(file.release()))
    {
        interface.size  = &size;
        interface.seek  = &seek;
        interface.read  = &read;
        interface.write = &write;
        interface.close = &close;

        stream = SDL_OpenIO(&interface, this);
    }

    sdl_io_stream::~sdl_io_stream()
    {
        SDL_CloseIO(stream);
    }

    Sint64 sdl_io_stream::size(void* userdata)
    {
        sdl_io_stream* instance = reinterpret_cast<sdl_io_stream*>(userdata);
        return instance->file->total_size();
    }

    Sint64 sdl_io_stream::seek(void* userdata, Sint64 offset, int whence)
    {
        sdl_io_stream* instance = reinterpret_cast<sdl_io_stream*>(userdata);
        auto           std_seekdir_it = sdl_seekdirs.find(whence);
        if (std_seekdir_it != sdl_seekdirs.end())
        {
            instance->io.seekg(offset, std_seekdir_it->second);
            instance->io.seekp(offset, std_seekdir_it->second);
            return instance->io.tellp();
        }
        return -1;
    }

    size_t sdl_io_stream::read(void*         userdata,
                               void*         ptr,
                               size_t        size,
                               SDL_IOStatus* status)
    {
        sdl_io_stream* instance = reinterpret_cast<sdl_io_stream*>(userdata);
        auto           start    = instance->io.tellg();
        instance->io.read(static_cast<char*>(ptr), size);

        if (status)
        {
            if (instance->io.good())
            {
                *status = SDL_IO_STATUS_READY;
            }
            else if (instance->io.eof())
            {
                *status = SDL_IO_STATUS_EOF;
            }
            else
            {
                *status = SDL_IO_STATUS_ERROR;
            }
        }

        size_t result = instance->io.tellg() - start;

        return result;
    }

    size_t sdl_io_stream::write(void*         userdata,
                                const void*   ptr,
                                size_t        size,
                                SDL_IOStatus* status)
    {
        sdl_io_stream* instance = reinterpret_cast<sdl_io_stream*>(userdata);
        auto           start    = instance->io.tellp();
        instance->io.write(static_cast<const char*>(ptr), size);

        if (status)
        {
            if (instance->io.good())
            {
                *status = SDL_IO_STATUS_READY;
            }
            else if (instance->io.eof())
            {
                *status = SDL_IO_STATUS_EOF;
            }
            else
            {
                *status = SDL_IO_STATUS_ERROR;
            }
        }

        size_t result = instance->io.tellg() - start;

        return result;
    }

    int sdl_io_stream::close(void* userdata)
    {
        sdl_io_stream* instance = reinterpret_cast<sdl_io_stream*>(userdata);
        instance->io.flush();
        instance->file->close();
        if (!instance->io.good())
        {
            return -1;
        }
        return 0;
    }

} // namespace Kengine::file_manager
