#pragma once

#include "Kengine/event/event.hxx"
#include "Kengine/imgui/imgui-editable.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "Kengine/string/string-id.hxx"

#include <string_view>

namespace Kengine
{
    class component : public serializable, public imgui::editable
    {
    public:
        component(std::string_view name);
        ~component();

        inline string_id get_name_id() const { return name_id; };

    private:
        string_id name_id;
    };
} // namespace Kengine