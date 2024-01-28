#include "Kengine/resources/resource-manager.hxx"

#include "Kengine/io/file-manager.hxx"
#include "Kengine/log/log.hxx"

#include <unordered_map>

namespace Kengine::resource_manager
{
    static std::unordered_map<string_id, res_weak_ptr<resource>>
        registered_resources;

    void initialize() {}

    void shutdown()
    {
        while (registered_resources.size() > 0)
        {
            registered_resources.begin()->second.counter->free();
        }
    }

    res_ptr<resource> load_resource(path res_path)
    {
        auto         res_file = file_manager::load_file(res_path);
        std::istream res_is(res_file.get());

        res_ptr<resource> loaded_res = nullptr;
        std::size_t       size = resource::deserialize(res_is, loaded_res);
        if (size)
            return loaded_res;

        return nullptr;
    }

    void save_resource(path res_path, res_ptr<resource> res)
    {
        if (res)
        {
            auto res_file =
                file_manager::open_file(res_path, std::ios_base::out);
            std::ostream res_os(res_file.get());

            resource::serialize(res_os, res);
        }
    }

    res_ptr<resource> get_resource(string_id r_id)
    {
        auto resource = registered_resources.find(r_id);
        if (resource != registered_resources.end())
            return resource->second.lock();
        else
            return nullptr;
    }

    void registrate_resource(const res_ptr<resource>& res)
    {
        if (res)
        {
            string_id r_id = res->get_resource_id();

            KENGINE_ASSERT(!registered_resources.contains(r_id),
                           "There's another resource with such id to add");

            KENGINE_ASSERT(r_id, "Invalid resource.");

            if (r_id)
                registered_resources[r_id] = res_weak_ptr<resource>(res);
        }
    }

    void remove_resource(string_id r_id)
    {
        KENGINE_ASSERT(registered_resources.contains(r_id),
                       "There's no resource with such id to remove");
        if (registered_resources.contains(r_id))
            registered_resources.erase(r_id);
    }

} // namespace Kengine::resource_manager