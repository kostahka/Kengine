#include "opengl-debug.hxx"

#undef NDEBUG
#include "Kengine/log/log.hxx"

namespace Kengine::opengl_debug
{
    bool initialize(int gl_major_version, int gl_minor_version, int gl_profile)
    {
        if ((((gl_major_version == 3 && gl_minor_version >= 2) ||
              (gl_major_version > 3)) &&
             gl_profile == SDL_GL_CONTEXT_PROFILE_ES) ||
            (((gl_major_version == 4 && gl_minor_version >= 3) ||
              (gl_major_version > 4)) &&
             gl_profile == SDL_GL_CONTEXT_PROFILE_CORE))
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(&Kengine::opengl_debug::debug_message,
                                   nullptr);
            glDebugMessageControl(
                GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            glDebugMessageControl(GL_DONT_CARE,
                                  GL_DONT_CARE,
                                  GL_DEBUG_SEVERITY_NOTIFICATION,
                                  0,
                                  nullptr,
                                  GL_FALSE);

            return true;
        }

        return false;
    }

    void APIENTRY debug_message(GLenum        source,
                                GLenum        type,
                                GLuint        id,
                                GLenum        severity,
                                GLsizei       length,
                                const GLchar* message,
                                const void*   userParam)
    {
        std::string mess(message, length);
        KENGINE_DEBUG("{}", mess.c_str());
    }
} // namespace Kengine::opengl_debug