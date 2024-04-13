#pragma once

#include "Kengine/components/component-info.hxx"
#include "Kengine/components/component.hxx"
#include "Kengine/resources/font-resource.hxx"
#include "Kengine/units/rect.hxx"

#include "entt/entt.hpp"

#include <string>

namespace Kengine
{
    enum class text_align
    {
        center,
        left,
        right
    };

    struct E_DECLSPEC text_component : public component
    {
        static constexpr auto name = "text_component";

        text_component();

        text_component(text_component& other);
        text_component(text_component&& other);

        text_component& operator=(text_component& other);
        text_component& operator=(text_component&& other);

        ~text_component();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

        void set_font(const res_ptr<font_resource>& font);

        inline const res_ptr<font_resource>& get_font() const { return font; }

        inline res_ptr<font_resource>& get_font() { return font; }

        std::u32string text         = U"";
        float          line_advance = 1.f;
        float          text_size    = 1.f;
        text_align     align        = text_align::center;
        vec4           color        = { 1, 1, 1, 1 };
        vec2           origin       = { 0, 0 };
        float          angle        = 0;
        vec2           scale{ 1, 1 };
        int            layer = 0;
        string_id      gui_event_id{};

        bool is_hovered = false;

    private:
        res_ptr<font_resource> font = nullptr;
    };

} // namespace Kengine