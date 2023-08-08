#pragma once

#include "SDL3/SDL_render.h"

#include "Kengine/log/log.hxx"

#ifdef __ANDROID__
 #define KENGINE_GL_GLES
#else
 #define KENGINE_GL_GLAD
#endif

#ifdef KENGINE_GL_GLES
 #include <GLES3/gl3.h>
#endif

#ifdef KENGINE_GL_GLAD
 #include <glad/glad.h>
#endif

#define KENGINE_GL_CHECK(EXP)                                                  \
    EXP;                                                                       \
    KENGINE_ASSERT(Kengine::opengl::gl_check(), "GL error")

namespace Kengine::opengl
{
    [[nodiscard]] bool initialize();
    [[nodiscard]] bool extension_support(const char* name);

    bool gl_check();
} // namespace Kengine::opengl