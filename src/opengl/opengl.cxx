#include "opengl.hxx"

#include <algorithm>
#include <vector>

#include "Kengine/log/log.hxx"

namespace Kengine::opengl
{
    std::vector<const char*> gl_extensions;
    bool                     initialized = false;

    static std::unordered_map<texture_filter, GLint> texture_filter_gl{
        {texture_filter::linear,                  GL_LINEAR                },
        { texture_filter::nearest,                GL_NEAREST               },
        { texture_filter::nearest_mipmap_nearest, GL_NEAREST_MIPMAP_NEAREST},
        { texture_filter::linear_mipmap_nearest,  GL_LINEAR_MIPMAP_NEAREST },
        { texture_filter::nearest_mipmap_linear,  GL_NEAREST_MIPMAP_LINEAR },
        { texture_filter::linear_mipmap_linear,   GL_LINEAR_MIPMAP_LINEAR  },
    };

    static std::unordered_map<texture_format, GLint> texture_format_gl{
        {texture_format::red,            GL_RED            },
        { texture_format::rg,            GL_RG             },
        { texture_format::rgb,           GL_RGB            },
        { texture_format::rgba,          GL_RGBA           },
        { texture_format::depth,         GL_DEPTH_COMPONENT},
        { texture_format::depth_stencil, GL_DEPTH_STENCIL  },
    };

    static std::unordered_map<GLint, texture_format> gl_texture_format{
        {GL_RED,                texture_format::red          },
        { GL_RG,                texture_format::rg           },
        { GL_RGB,               texture_format::rgb          },
        { GL_RGBA,              texture_format::rgba         },
        { GL_DEPTH_COMPONENT16, texture_format::depth        },
        { GL_DEPTH_STENCIL,     texture_format::depth_stencil},
    };

    static std::unordered_map<texture_internal_format, GLenum>
        texture_internal_format_gl{
            {texture_internal_format::r8,                 GL_R8                },
            { texture_internal_format::r8ui,              GL_R8UI              },
            { texture_internal_format::r8i,               GL_R8I               },
            { texture_internal_format::r16ui,             GL_R16UI             },
            { texture_internal_format::r16i,              GL_R16I              },
            { texture_internal_format::r32ui,             GL_R32UI             },
            { texture_internal_format::r32i,              GL_R32I              },

            { texture_internal_format::rg8,               GL_RG8               },
            { texture_internal_format::rg8ui,             GL_RG8UI             },
            { texture_internal_format::rg8i,              GL_RG8I              },
            { texture_internal_format::rg16ui,            GL_RG16UI            },
            { texture_internal_format::rg16i,             GL_RG16I             },
            { texture_internal_format::rg32ui,            GL_RG32UI            },
            { texture_internal_format::rg32i,             GL_RG32I             },

            { texture_internal_format::rgb8,              GL_RGB8              },
            { texture_internal_format::rgb565,            GL_RGB565            },

            { texture_internal_format::srgb8_a8,          GL_SRGB8_ALPHA8      },

            { texture_internal_format::rgba8,             GL_RGBA8             },
            { texture_internal_format::rgb5_a1,           GL_RGB5_A1           },
            { texture_internal_format::rgba4,             GL_RGBA4             },
            { texture_internal_format::rgb10_a2,          GL_RGB10_A2          },
            { texture_internal_format::rgba8ui,           GL_RGBA8UI           },
            { texture_internal_format::rgba8i,            GL_RGBA8I            },
            { texture_internal_format::rgb10_a2ui,        GL_RGB10_A2UI        },
            { texture_internal_format::rgba16ui,          GL_RGBA16UI          },
            { texture_internal_format::rgba16i,           GL_RGBA16I           },
            { texture_internal_format::rgba32ui,          GL_RGBA32UI          },
            { texture_internal_format::rgba32i,           GL_RGBA32I           },

            { texture_internal_format::depth16,           GL_DEPTH_COMPONENT16 },
            { texture_internal_format::depth24,           GL_DEPTH_COMPONENT24 },
            { texture_internal_format::depth32f,          GL_DEPTH_COMPONENT32F},
            { texture_internal_format::depth24_stencil8,  GL_DEPTH24_STENCIL8  },
            { texture_internal_format::depth32f_stencil8,
             GL_DEPTH32F_STENCIL8                                              },
            { texture_internal_format::stencil8,          GL_STENCIL_INDEX8    },
    };

    static std::unordered_map<GLenum, const char*> framebuffer_status_str{
        {GL_FRAMEBUFFER_COMPLETE,                       "GL_FRAMEBUFFER_COMPLETE"   },
        { GL_FRAMEBUFFER_UNDEFINED,                     "GL_FRAMEBUFFER_UNDEFINED"  },
        { GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
         "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"                                     },
        { GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
         "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"                             },
        { GL_FRAMEBUFFER_UNSUPPORTED,                   "GL_FRAMEBUFFER_UNSUPPORTED"},
        { GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
         "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"                                    },
#ifndef __ANDROID__
        { GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
         "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"                                  },
        { GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
                "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"                                    },
        { GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
                "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"                                    },
#endif
    };

    bool initialize()
    {
        if (!initialized)
        {
#ifdef KENGINE_GL_GLAD
            auto load_gl_func = [](const char* func_name) {
                return reinterpret_cast<void*>(
                    SDL_GL_GetProcAddress(func_name));
            };
            if (gladLoadGLES2Loader(load_gl_func) == 0)
            {
                KENGINE_ERROR("Failed to initialize glad");
                return false;
            }
#endif
            GLint n = 0;
            glGetIntegerv(GL_NUM_EXTENSIONS, &n);

            for (GLint i = 0; i < n; i++)
            {
                auto ext = (const char*)glGetStringi(GL_EXTENSIONS, i);
                gl_extensions.push_back(ext);
                // KENGINE_INFO("Ext {}: {}", i, ext);
            }
            initialized = true;
        }
        return true;
    }

    bool extension_support(const char* name)
    {
        auto ext = std::find(gl_extensions.begin(), gl_extensions.end(), name);
        return ext != gl_extensions.end();
    }

    bool gl_check()
    {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
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
            return false;
        }
        return true;
    };

    GLint get_gl_texture_filter(texture_filter filter)
    {
        auto gl_filter = texture_filter_gl.find(filter);
        if (gl_filter != texture_filter_gl.end())
            return gl_filter->second;
        else
            return 0;
    }

    GLint get_gl_texture_format(texture_format format)
    {
        auto gl_format = texture_format_gl.find(format);
        if (gl_format != texture_format_gl.end())
            return gl_format->second;
        else
            return 0;
    }

    GLenum get_gl_texture_internal_format(texture_internal_format format)
    {
        auto internal_format = texture_internal_format_gl.find(format);
        if (internal_format != texture_internal_format_gl.end())
            return internal_format->second;
        else
            return 0;
    }

    const char* get_framebuffer_status_str(GLenum status)
    {
        auto str = framebuffer_status_str.find(status);
        if (str != framebuffer_status_str.end())
            return str->second;
        else
            return "<STATUS NOT FOUND>";
    }

    texture_format get_texture_format_gl(GLint gl_format)
    {
        auto format = gl_texture_format.find(gl_format);
        if (format != gl_texture_format.end())
            return format->second;
        else
            return texture_format::none;
    }
} // namespace Kengine::opengl