#pragma once

#include "Kengine/resources/res-ptr.hxx"

#include <filesystem>

namespace Kengine::imgui
{
    E_DECLSPEC bool edit_file(const char* name, std::filesystem::path& path);

    E_DECLSPEC bool edit_resource(const char* name, res_ptr<resource>* res);

    template <typename ResourceType>
    bool edit_resource(const char* name, res_ptr<ResourceType>* res)
    {
        return edit_resource(name, reinterpret_cast<res_ptr<resource>*>(res));
    }
} // namespace Kengine::imgui