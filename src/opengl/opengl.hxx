#pragma once

#include "SDL3/SDL_render.h"

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

namespace Kengine::opengl
{
    bool initialize();
    bool extension_support(const char *name);
} // namespace Kengine::opengl