#pragma once

#include <filesystem>
#include <memory>
#include <streambuf>

namespace Kengine::file_manager
{
    struct file_buffer : public std::streambuf
    {
        static std::unique_ptr<file_buffer> get_empty();

        inline char* wbegin() const { return write_buffer.get(); }

        inline char* rbegin() const { return read_buffer.get(); }

        inline size_t size() const { return buf_size; }

        inline bool is_open() { return is_file_open; };

        virtual bool open(std::filesystem::path   path,
                          std::ios_base::openmode mode,
                          size_t                  buf_size = 10) = 0;
        virtual void close()                    = 0;

        virtual ~file_buffer();

    protected:
        file_buffer();

        std::unique_ptr<char[]> write_buffer;
        std::unique_ptr<char[]> read_buffer;
        size_t                  buf_size;
        bool                    is_file_open;
        size_t                  file_size;
    };

    std::unique_ptr<file_buffer> load_file(std::filesystem::path path);
    std::unique_ptr<file_buffer> open_file(std::filesystem::path   path,
                                           std::ios_base::openmode mode,
                                           size_t buf_size = 0);

    void                  set_base_path(std::filesystem::path path);
    std::filesystem::path get_base_path();

    bool file_exists(std::filesystem::path path);
} // namespace Kengine::file_manager
