#include "Kengine/graphics/render-pass.hxx"

#include "Kengine/graphics/graphics.hxx"

#include <algorithm>

namespace Kengine::graphics
{
    render_pass::render_pass() {}

    void render_pass::add_render_packet(const render_packet& packet)
    {
        if (packet.vao && packet.get_material())
        {
            packets.push_back(std::move(packet));
        }
    }

    void render_pass::render()
    {
        std::sort(packets.begin(),
                  packets.end(),
                  [](const render_packet& left, const render_packet& right)
                  { return left.layer < right.layer; });
        for (auto packet_it = packets.begin(); packet_it != packets.end();
             ++packet_it)
        {
            render_packet& packet = *packet_it;
            packet.vao->bind();
            bind_material(packet.get_material());
            if (packet.instances_count > 1)
            {
                packet.vao->draw_instanced(packet.mode,
                                           packet.instances_count,
                                           packet.vertices_count,
                                           packet.vertices_start);
            }
            else
            {
                packet.vao->draw(
                    packet.mode, packet.vertices_count, packet.vertices_start);
            }
            packet.vao->unbind();
        }
        packets.clear();
    }
} // namespace Kengine::graphics