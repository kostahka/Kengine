#include "Kengine/resources/resource.hxx"

#include "Kengine/file-last-modify-listener.hxx"
#include "Kengine/resources/resource-manager.hxx"

namespace Kengine
{
    void resource::take_data()
    {
        if (!p_count)
        {
            this->load_data();
        }
        p_count++;
    }

    void resource::free_data()
    {
        if (p_count == 1)
        {
            p_count--;
            this->unload_data();
        }
    }

    resource::resource(resource_type r_type, path f_path, std::string_view name)
        : r_type(r_type)
        , f_path(f_path)
        , p_count(0)
    {
        r_id = save_string(name.data());
    }

    resource::resource(resource_type r_type, std::string_view name)
        : r_type(r_type)
        , f_path(path())
        , p_count(0)
    {
        r_id = save_string(name.data());
    }

    resource::~resource()
    {
        resource_manager::remove_resource(r_id);
    }
} // namespace Kengine