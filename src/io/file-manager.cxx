#include "Kengine/io/file-manager.hxx"

#include "SDL3/SDL_filesystem.h"
#include <SDL_error.h>
#include <SDL_rwops.h>

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
        {std::ios_base::beg,  SDL_RW_SEEK_SET},
        { std::ios_base::end, SDL_RW_SEEK_END},
        { std::ios_base::cur, SDL_RW_SEEK_CUR}
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

    struct file_buffer_impl : public file_buffer
    {
        file_buffer_impl()
            : file_buffer()
            , file(nullptr)
            , file_write_pos(0)
            , file_read_pos(0)
            , file_curr_pos(0)
        {
            setg(nullptr, nullptr, nullptr);
            setp(nullptr, nullptr);
        }

        bool open(std::filesystem::path   path,
                  std::ios_base::openmode mode,
                  size_t                  buf_size) override
        {
            auto sdl_openmode_map = openmodes.find(mode);
            if (sdl_openmode_map == openmodes.end())
            {
                KENGINE_ERROR("Undefined openmode");
                return false;
            }
            file =
                SDL_RWFromFile(path.string().c_str(), sdl_openmode_map->second);
            if (!file)
            {
                auto  path_wstr    = path.c_str();
                auto  path_str_len = std::wcslen(path_wstr);
                char* path_str     = new char[path_str_len];
                wcstombs_s(
                    nullptr, path_str, path_str_len, path_wstr, path_str_len);
                KENGINE_ERROR("Failed to open file [{}], error: {}",
                              path_str,
                              SDL_GetError());
                return false;
            }
            is_file_open = true;
            if (buf_size)
            {
                this->buf_size = buf_size;
            }
            else
            {
                this->buf_size = 10;
            }
            file_size      = file->size(file);
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

            return true;
        }

        bool load(std::filesystem::path path)
        {
            file = SDL_RWFromFile(path.string().c_str(), "rb");
            if (!file)
            {
                auto  path_wstr    = path.c_str();
                auto  path_str_len = std::wcslen(path_wstr);
                char* path_str     = new char[path_str_len];
                wcstombs_s(
                    nullptr, path_str, path_str_len, path_wstr, path_str_len);
                KENGINE_ERROR("Failed to open file [{}], error: {}",
                              path_str,
                              SDL_GetError());
                return false;
            }
            is_file_open   = true;
            file_size      = file->size(file);
            file_curr_pos  = 0;
            file_write_pos = file_read_pos = file_curr_pos;

            buf_size = file_size;

            read_buffer = std::make_unique<char[]>(buf_size);
            setg(read_buffer.get(),
                 read_buffer.get() + buf_size,
                 read_buffer.get() + buf_size);

            if (sync() != 0)
            {
                close();
                return false;
            }

            return true;
        }

        void close() override
        {
            if (file)
            {
                if (0 != file->close(file))
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

        ~file_buffer_impl() override { close(); }

    protected:
        int sync() override
        {
            if (!file)
                return -1;

            if (pptr() != pbase())
            {
                if (file_curr_pos != file_write_pos)
                {
                    file_curr_pos =
                        file->seek(file, file_write_pos, SDL_RW_SEEK_SET);
                    if (file_curr_pos == -1)
                    {
                        KENGINE_ERROR("Can't write to file.");
                        return -1;
                    }
                }
                size_t must_write_bytes = pptr() - pbase();
                Sint64 write_bytes =
                    file->write(file, write_buffer.get(), must_write_bytes);
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
                if (file_curr_pos != file_read_pos)
                {
                    file_curr_pos =
                        file->seek(file, file_read_pos, SDL_RW_SEEK_SET);
                    if (file_curr_pos == -1)
                    {
                        KENGINE_ERROR("Can't read from file.");
                        return -1;
                    }
                }
                Sint64 read_bytes =
                    file->read(file, read_buffer.get(), buf_size);
                if (read_bytes == -1)
                {
                    KENGINE_ERROR("Can't read from file.");
                    return -1;
                }

                setg(read_buffer.get(),
                     read_buffer.get(),
                     read_buffer.get() + buf_size);

                if (read_bytes < static_cast<Sint64>(buf_size))
                {
                    read_buffer.get()[read_bytes] = traits_type::eof();
                }
                file_read_pos += read_bytes;
                file_curr_pos = file_read_pos;
            }

            return 0;
        }

        int_type overflow(int_type c) override
        {
            if (!pptr())
                return traits_type::eof();

            if (c == traits_type::eof())
                return traits_type::eof();

            if (sync() != 0)
                return traits_type::eof();

            *pptr() = c;
            return c;
        }

        int_type underflow() override
        {
            if (!gptr())
                return traits_type::eof();

            if (sync() != 0)
                return traits_type::eof();

            return *gptr();
        }

        pos_type seekoff(off_type                off,
                         std::ios_base::seekdir  dir,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override
        {
            if (sync() != 0)
                return -1;

            switch (dir)
            {
                case std::ios_base::beg:
                    if (which & std::ios_base::in)
                        file_read_pos = off;
                    if (which & std::ios_base::out)
                        file_write_pos = off;
                    break;
                case std::ios_base::end:
                    if (which & std::ios_base::in)
                        file_read_pos = file_size + off;
                    if (which & std::ios_base::out)
                        file_write_pos = file_size + off;
                    break;
                case std::ios_base::cur:
                    if (which & std::ios_base::in)
                        file_read_pos += off;
                    if (which & std::ios_base::out)
                        file_write_pos += off;
                    break;
                default:
                    return -1;
                    break;
            }
            return 0;
        }

        pos_type seekpos(pos_type                pos,
                         std::ios_base::openmode which =
                             std::ios_base::in | std::ios_base::out) override
        {
            return seekoff(pos, std::ios_base::beg, which);
        }

    private:
        SDL_RWops* file;
        size_t     file_write_pos;
        size_t     file_read_pos;
        size_t     file_curr_pos;
    };

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
} // namespace Kengine::file_manager
