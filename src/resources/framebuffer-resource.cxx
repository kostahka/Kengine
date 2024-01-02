#include "Kengine/resources/framebuffer-resource.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/resources/res-ptr.hxx"

namespace Kengine
{
    framebuffer_resource::framebuffer_resource(
        res_ptr<resource> color_attachment,
        res_ptr<resource> depth_stencil_attachment,
        const vec4&       clear_color,
        std::string_view  name)
        : resource(resource_type::framebuffer, name)
        , color_attachment(color_attachment)
        , depth_stencil_attachment(depth_stencil_attachment)
        , id(0)
        , color_id(0)
        , depth_stencil_id(0)
        , color_attach(false)
        , depth_attach(false)
        , stencil_attach(false)
        , clear_color(clear_color)
    {
        KENGINE_ASSERT(!color_attachment ||
                           color_attachment->get_resource_type() ==
                               resource_type::texture ||
                           color_attachment->get_resource_type() ==
                               resource_type::renderbuffer,
                       "Color attachment can be only texture or renderbuffer");

        KENGINE_ASSERT(!depth_stencil_attachment ||
                           depth_stencil_attachment->get_resource_type() ==
                               resource_type::texture ||
                           depth_stencil_attachment->get_resource_type() ==
                               resource_type::renderbuffer,
                       "Color attachment can be only texture or renderbuffer");

        if (color_attachment->get_resource_type() == resource_type::texture)
        {
            size = static_resource_cast<texture_resource>(color_attachment)
                       ->get_size();
        }
        else
        {
            size = static_resource_cast<renderbuffer_resource>(color_attachment)
                       ->get_size();
        }

        if (depth_stencil_attachment->get_resource_type() ==
            resource_type::texture)
        {
            KENGINE_ASSERT(size == static_resource_cast<texture_resource>(
                                       depth_stencil_attachment)
                                       ->get_size(),
                           "Attachments not the same size for framebuffer");
        }
        else
        {
            KENGINE_ASSERT(size == static_resource_cast<renderbuffer_resource>(
                                       depth_stencil_attachment)
                                       ->get_size(),
                           "Attachments not the same size for framebuffer");
        }
    }

    framebuffer_resource::~framebuffer_resource()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteFramebuffers(1, &id));
            KENGINE_INFO("Unloaded framebuffer: {}", id);
        }

        if (color_attachment)
            color_attachment->free_data();
        if (depth_stencil_attachment)
            depth_stencil_attachment->free_data();
    }

    void framebuffer_resource::load_data()
    {
        if (color_attachment)
            color_attachment->take_data();
        if (depth_stencil_attachment)
            depth_stencil_attachment->take_data();

        KENGINE_GL_CHECK(glGenFramebuffers(1, &id));
        KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, id));

        attachment(color_attachment, true);
        attachment(depth_stencil_attachment, false);

        int32_t complete_status =
            KENGINE_GL_CHECK(glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (complete_status != GL_FRAMEBUFFER_COMPLETE)
        {
            KENGINE_ERROR("Failed to create framebuffer. Complete status: {}",
                          opengl::get_framebuffer_status_str(complete_status));
        }
        else
        {
            KENGINE_INFO("Loaded framebuffer: {}", id);
        }

        KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    void framebuffer_resource::unload_data()
    {
        if (id)
        {
            KENGINE_GL_CHECK(glDeleteFramebuffers(1, &id));
            KENGINE_INFO("Unloaded framebuffer: {}", id);
            id = 0;
        }

        if (color_attachment)
            color_attachment->free_data();
        if (depth_stencil_attachment)
            depth_stencil_attachment->free_data();

        color_id         = 0;
        depth_stencil_id = 0;

        color_attach   = false;
        depth_attach   = false;
        stencil_attach = false;
    }

    void framebuffer_resource::attachment(res_ptr<resource> attach,
                                          bool              color_attachment)
    {
        if (!attach)
            return;

        GLenum   gl_format = 0;
        uint32_t attach_id = 0;

        resource_type res_type = attach->get_resource_type();
        if (res_type == resource_type::texture)
        {

            auto format =
                static_cast<texture_resource*>(attach.get())->get_format();

            gl_format = opengl::get_gl_texture_format(format);

            attach_id = static_cast<texture_resource*>(attach.get())->get_id();
        }
        else if (res_type == resource_type::renderbuffer)
        {
            auto format =
                static_cast<renderbuffer_resource*>(attach.get())->get_format();

            gl_format = opengl::get_gl_texture_internal_format(format);

            attach_id =
                static_cast<renderbuffer_resource*>(attach.get())->get_id();
        }

        if (color_attachment)
            color_id = attach_id;
        else
            depth_stencil_id = attach_id;

        KENGINE_ASSERT(attach_id != 0,
                       "Attachment not texture or renderbuffer");

        GLenum attach_type;

        if (color_attachment)
        {
            attach_type  = GL_COLOR_ATTACHMENT0;
            color_attach = true;
        }
        else
        {
            if (gl_format == GL_DEPTH24_STENCIL8 ||
                gl_format == GL_DEPTH32F_STENCIL8 ||
                gl_format == GL_DEPTH_STENCIL)
            {
                attach_type    = GL_DEPTH_STENCIL_ATTACHMENT;
                stencil_attach = depth_attach = true;
            }
            else
            {
                attach_type  = GL_DEPTH_ATTACHMENT;
                depth_attach = true;
            }
        }

        if (res_type == resource_type::texture)
        {
            KENGINE_GL_CHECK(glFramebufferTexture2D(
                GL_FRAMEBUFFER, attach_type, GL_TEXTURE_2D, attach_id, 0));
        }
        else
        {
            KENGINE_GL_CHECK(glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, attach_type, GL_RENDERBUFFER, attach_id));
        }
    }

    void framebuffer_resource::set_clear_color(const vec4& clear_color)
    {
        this->clear_color = clear_color;
    }

} // namespace Kengine