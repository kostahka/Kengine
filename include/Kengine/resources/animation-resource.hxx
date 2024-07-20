#pragma once

#include "Kengine/resources/resource.hxx"
#include "Kengine/string/string-id.hxx"
#include "Kengine/types/rect.hxx"

#include <string_view>
#include <unordered_map>
#include <vector>

namespace Kengine
{
    class E_DECLSPEC animation_resource : public resource
    {
    public:
        animation_resource(std::string_view name);
        ~animation_resource();

        void add_animation(string_id id, std::vector<rect> uvs);
        void remove_animation(string_id id);

        inline const std::unordered_map<string_id, std::vector<rect>>&
        get_animations() const
        {
            return animations;
        }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;
        std::size_t serialize_size() const override;

        bool imgui_editable_render() override;

    protected:
        void load_data() override;
        void unload_data() override;

        std::unordered_map<string_id, std::vector<rect>> animations{
            { string_id(), { { 0, 0, 1, 1 } } }
        };
    };
} // namespace Kengine