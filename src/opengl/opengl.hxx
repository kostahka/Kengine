#pragma once

#include "SDL3/SDL_render.h"

#include "Kengine/graphics/texture-common.hxx"
#include "Kengine/log/log.hxx"

#ifdef __ANDROID__
 #define KENGINE_GL_GLES
#else
 #define KENGINE_GL_GLAD
#endif

#ifdef KENGINE_GL_GLES
 #include <GLES3/gl32.h>
#endif

#ifdef KENGINE_GL_GLAD
 #include <glad/glad.h>
#endif

#define KENGINE_GL_CHECK(EXP)                                                  \
    EXP;                                                                       \
    KENGINE_ASSERT_WARN(Kengine::opengl::gl_check(), "GL error")

namespace Kengine::opengl
{
    extern bool initialized;

    [[nodiscard]] bool initialize();
    [[nodiscard]] bool extension_support(const char* name);

    bool gl_check();

    [[nodiscard]] GLint  get_gl_texture_filter(texture_filter filter);
    [[nodiscard]] GLint  get_gl_texture_format(texture_format format);
    [[nodiscard]] GLenum get_gl_texture_internal_format(
        texture_internal_format format);
    [[nodiscard]] const char*    get_framebuffer_status_str(GLenum status);
    [[nodiscard]] texture_format get_texture_format_gl(GLint gl_format);
} // namespace Kengine::opengl