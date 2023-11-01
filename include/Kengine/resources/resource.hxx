#pragma once

#include "Kengine/io/file-manager.hxx"
#include "Kengine/string/string-id.hxx"
#include "resource_types.hxx"

#include <filesystem>
#include <string_view>

namespace Kengine
{
    using std::filesystem::path;

    class resource
    {
    public:
        void take_data(); // Own resource. Load resource data if needed
        void free_data(); // Deown resource.
                          // Unload resource if nobody else own it.

        virtual ~resource();

        [[nodiscard]] inline resource_type get_resource_type() const
        {
            return r_type;
        }

        [[nodiscard]] inline string_id get_resource_id() const { return r_id; }

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

} // namespace Kengine