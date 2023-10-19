#include "Kengine/render/texture.hxx"
#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"

#include <cstddef>
#include <fstream>
#include <ios>
#include <vector>

int decodePNG(std::vector<unsigned char>& out_image,
              unsigned long&              image_width,
              unsigned long&              image_height,
              const unsigned char*        in_png,
              size_t                      in_size,
              bool                        convert_to_rgba32 = true);

namespace Kengine
{

    class texture_impl : public texture_object
    {
    public:
        texture_impl(std::string texture_path)
        {
            auto texture_file = file_manager::load_file(texture_path);

            std::vector<unsigned char> image_memory;
            unsigned long              image_width  = 0;
            unsigned long              image_height = 0;

            if (decodePNG(
                    image_memory,
                    image_width,
                    image_height,
                    reinterpret_cast<unsigned char*>(texture_file->rbegin()),
                    texture_file->size(),
                    true))
            {
                KENGINE_ERROR("Failed to decode png texture from [{}]",
                              texture_path);
            }
            size = { static_cast<int>(image_width),
                     static_cast<int>(image_height) };

            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);

            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         static_cast<GLsizei>(image_width),
                         static_cast<GLsizei>(image_height),
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         &image_memory[0]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        };

        void bind() override { glBindTexture(GL_TEXTURE_2D, texture); };

        [[nodiscard]] ivec2 get_size() const override { return size; };

        [[nodiscard]] intptr_t get_id() const override { return texture; };

        ~texture_impl() override { glDeleteTextures(1, &texture); };

    private:
        ivec2  size;
        GLuint texture;
    };

    texture_object* create_texture(std::string texture_path)
    {
        return new texture_impl(texture_path);
    }
} // namespace Kengine
