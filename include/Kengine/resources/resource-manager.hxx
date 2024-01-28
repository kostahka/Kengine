#pragma once

#include "res-ptr.hxx"

namespace Kengine::resource_manager
{
    res_ptr<resource> get_resource(string_id r_id);

    void registrate_resource(const res_ptr<resource>& resource);
    void remove_resource(string_id r_id);

    res_ptr<resource> load_resource(path res_path);
    void              save_resource(path res_path, res_ptr<resource> res);
} // namespace Kengine::resource_manager

namespace Kengine
{
    template <class ResourceType, typename... Types>
    res_ptr<ResourceType> make_resource(Types&&... args)
    {
        ResourceType* res = new ResourceType(std::forward<Types>(args)...);
        res_ptr<ResourceType> ptr(res);
        resource_manager::registrate_resource(
            static_resource_cast<resource, ResourceType>(ptr));
        return ptr;
    }
} // namespace Kengine