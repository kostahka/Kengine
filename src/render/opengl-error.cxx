#include "opengl-error.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/log/log.hxx"

namespace Kengine
{

    void gl_get_error(int line, const char* file)
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            KENGINE_ERROR("GL error at line {}, at file {}", line, file);
            switch (error)
            {
                case GL_INVALID_ENUM:
                    KENGINE_ERROR("GL_INVALID_ENUM");
                    break;
                case GL_INVALID_VALUE:
                    KENGINE_ERROR("GL_INVALID_VALUE");
                    break;
                case GL_INVALID_OPERATION:
                    KENGINE_ERROR("GL_INVALID_OPERATION");
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    KENGINE_ERROR("GL_INVALID_FRAMEBUFFER_OPERATION");
                    break;
                case GL_OUT_OF_MEMORY:
                    KENGINE_ERROR("GL_OUT_OF_MEMORY");
                    break;
#ifndef __ANDROID__
                case GL_STACK_UNDERFLOW:
                    KENGINE_ERROR("GL_STACK_UNDERFLOW");
                    break;
                case GL_STACK_OVERFLOW:
                    KENGINE_ERROR("GL_STACK_OVERFLOW");
                    break;
#endif
            }
        }
    };
}; // namespace Kengine