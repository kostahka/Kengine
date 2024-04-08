#pragma once

namespace Kengine::imgui
{
    class E_DECLSPEC editable
    {
    public:
        virtual bool imgui_editable_render();
    };
} // namespace Kengine::imgui