#include "Kengine/graphics/render-manager.hxx"
#include "render-manager.hxx"

#include "../opengl/opengl-debug.hxx"
#include "../window/window.hxx"
#include "Kengine/log/log.hxx"
#include "Kengine/units/vector.hxx"

#include <stack>

namespace Kengine::graphics::render_manager
{
    vec4 clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };

    static std::stack<framebuffer> framebuffers{};

    bool initialize()
    {
        KENGINE_GL_CHECK(glEnable(GL_DEPTH_TEST));
        KENGINE_GL_CHECK(glDepthFunc(GL_ALWAYS));
        KENGINE_GL_CHECK(glEnable(GL_BLEND));
        KENGINE_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        KENGINE_GL_CHECK(glClearColor(
            clear_color.r, clear_color.g, clear_color.b, clear_color.a));

        return true;
    }

    void begin_render()
    {
        // TODO
        KENGINE_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void end_render()
    {
        // TODO
        SDL_GL_SwapWindow(window::window);
    }

    void set_clear_color(const vec4& col)
    {
        if (framebuffers.size() > 0)
        {
            framebuffers.top().set_clear_color(col);
        }
        else
        {
            clear_color = col;
        }
        KENGINE_GL_CHECK(glClearColor(col.r, col.g, col.b, col.a));
    }

    void push_framebuffer(framebuffer& frame)
    {
        framebuffers.push(frame);
        KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, frame.get_id()));
        frame.clear();
        auto frame_size = frame.get_size();
        KENGINE_GL_CHECK(glViewport(0, 0, frame_size.x, frame_size.y));
    }

    void pop_framebuffer()
    {
        framebuffers.pop();
        if (framebuffers.size() > 0)
        {
            KENGINE_GL_CHECK(
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.top().get_id()));
        }
        else
        {
            KENGINE_GL_CHECK(glClearColor(
                clear_color.r, clear_color.g, clear_color.b, clear_color.a));
            KENGINE_GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }
        update_viewport();
    }

    void update_viewport()
    {
        if (framebuffers.size() > 0)
        {
            auto frame_size = framebuffers.top().get_size();
            KENGINE_GL_CHECK(glViewport(0, 0, frame_size.x, frame_size.y));
        }
        else
        {
            auto wnd_size = window::get_size();
            KENGINE_GL_CHECK(glViewport(0, 0, wnd_size.x, wnd_size.y));
        }
    }
} // namespace Kengine::graphics::render_manager