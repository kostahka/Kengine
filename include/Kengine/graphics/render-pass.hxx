#pragma once

#include "Kengine/graphics/render-packet.hxx"

#include <unordered_map>
#include <vector>

namespace Kengine::graphics
{
    class E_DECLSPEC render_pass
    {
    public:
        render_pass();

        void add_render_packet(const render_packet& packet);

        void render();

    private:
        std::vector<render_packet> packets{};
    };
} // namespace Kengine::graphics