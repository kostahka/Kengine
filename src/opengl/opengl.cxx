#include "opengl.hxx"

#include <algorithm>
#include <iostream>
#include <vector>

namespace Kengine::opengl
{
    std::vector<const char *> gl_extensions;
    bool                      initialized = false;

    bool initialize()
    {
        if (!initialized)
        {
#ifdef KENGINE_GL_GLAD
            auto load_gl_func = [](const char *func_name) {
                return reinterpret_cast<void *>(
                    SDL_GL_GetProcAddress(func_name));
            };
            if (gladLoadGLES2Loader(load_gl_func) == 0)
            {
                std::cerr << "Failed to initialize glad" << std::endl;
                return false;
            }
#endif
            GLint n = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &n);

            for (GLint i = 0; i < n; i++)
            {
                auto ext = (const char *)glGetStringi(GL_EXTENSIONS, i);
                gl_extensions.push_back(ext);
                std::clog << "Ext " << i << ": " << ext << "\n";
            }
            initialized = true;
        }
        return true;
    }

    bool extension_support(const char *name)
    {
        auto ext = std::find(gl_extensions.begin(), gl_extensions.end(), name);
        return ext != gl_extensions.end();
    }
} // namespace Kengine::opengl