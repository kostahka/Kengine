#include "Kengine/resources/resource.hxx"

#include "Kengine/file-last-modify-listener.hxx"

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
        if (--p_count == 0)
        {
            this->unload_data();
        }
    }

    resource::resource(resource_type r_type, path f_path)
        : r_type(r_type)
        , f_path(f_path)
        , p_count(0)
    {
    }

    resource::resource(resource_type r_type)
        : r_type(r_type)
        , p_count(0)
    {
    }

} // namespace Kengine