#pragma once

#include "Kengine/io/file-manager.hxx"
#include "Kengine/string/string-id.hxx"
#include "resource_types.hxx"

#include <filesystem>
#include <vector>

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

    protected:
        virtual void load_data()   = 0; // What do to load data for resource
        virtual void unload_data() = 0; // What do to unload data

        resource(resource_type r_type, path f_path);
        resource(resource_type r_type);

        int           p_count;
        path          f_path;
        resource_type r_type;
    };

} // namespace Kengine