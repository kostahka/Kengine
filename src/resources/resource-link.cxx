#include "Kengine/resources/resource-link.hxx"

#include "Kengine/resources/resource-manager.hxx"

namespace Kengine
{
    resource_link::resource_link()
        : res(nullptr)
        , res_path("")
    {
    }

    resource_link::resource_link(res_ptr<resource> res, path res_path)
        : res(res)
        , res_path(res_path)
    {
    }

    std::size_t resource_link::serialize(std::ostream& os) const
    {
        std::size_t size = 0;

        if (res)
        {
            size += serialization::write(os, res);
            size += serialization::write(os, res_path.string());
        }

        return size;
    }

    std::size_t resource_link::deserialize(std::istream& is)
    {
        std::size_t size = 0;

        std::string res_path_str;
        res = nullptr;

        size += serialization::read(is, res);
        size += serialization::read(is, res_path_str);

        res_path = path(res_path_str);

        if (!res)
        {
            res = resource_manager::load_resource(res_path);
        }

        return size;
    }
} // namespace Kengine