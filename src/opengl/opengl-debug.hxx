#pragma once

#include "opengl.hxx"

namespace Kengine::opengl_debug
{
    bool initialize(int gl_major_version, int gl_minor_version, int gl_profile);
    void APIENTRY debug_message(GLenum        source,
                                GLenum        type,
                                GLuint        id,
                                GLenum        severity,
                                GLsizei       length,
                                const GLchar* message,
                                const void*   userParam);

} // namespace Kengine::opengl_debug