#pragma once

#include "res-ptr.hxx"

namespace Kengine::resource_manager
{
    E_DECLSPEC res_ptr<resource> get_resource(string_id r_id);

    E_DECLSPEC void registrate_resource(const res_ptr<resource>& resource);
    E_DECLSPEC void remove_resource(string_id r_id);

    E_DECLSPEC res_ptr<resource> load_resource(path res_path);
    E_DECLSPEC void              save_resource(res_ptr<resource> res);

    E_DECLSPEC path relative_assets_path(path full_path);
} // namespace Kengine::resource_manager

namespace Kengine
{
    template <class ResourceType, typename... Types>
    res_ptr<ResourceType> make_resource_from_file(path r_path, Types&&... args)
    {
        ResourceType* res = new ResourceType(std::forward<Types>(args)...);
        res_ptr<ResourceType> ptr(
            res, resource_manager::relative_assets_path(r_path));
        resource_manager::registrate_resource(
            static_resource_cast<resource, ResourceType>(ptr));
        return ptr;
    }

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