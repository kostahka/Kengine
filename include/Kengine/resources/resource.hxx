#pragma once

#include "Kengine/imgui/imgui-editable.hxx"
#include "Kengine/io/file-manager.hxx"
#include "Kengine/resources/resource-types.hxx"
#include "Kengine/serialization/serialization.hxx"
#include "Kengine/string/string-id.hxx"

#include <filesystem>
#include <string_view>

namespace Kengine
{
    template <typename T>
    class res_ptr;

    using std::filesystem::path;

    class E_DECLSPEC resource : public serializable, public imgui::editable
    {
    public:
        void take_data(); // Own resource. Load resource data if needed
        void free_data(); // Deown resource.
                          // Unload resource if nobody else own it.

        virtual ~resource();

        void reload_data();

        [[nodiscard]] inline resource_type get_resource_type() const
        {
            return r_type;
        }

        [[nodiscard]] inline string_id get_resource_id() const { return r_id; }

        [[nodiscard]] inline path get_file_path() const { return f_path; }

        std::size_t serialize(std::ostream& os) const = 0;
        std::size_t deserialize(std::istream& is)     = 0;

    protected:
        virtual void load_data()   = 0; // What do to load data for resource
        virtual void unload_data() = 0; // What do to unload data

        resource(resource_type r_type, path f_path, std::string_view name);
        resource(resource_type r_type, std::string_view name);

        int           p_count;
        path          f_path;
        resource_type r_type;
        string_id     r_id;
    };

    E_DECLSPEC const char* get_resource_type_str(resource_type);

} // namespace Kengine