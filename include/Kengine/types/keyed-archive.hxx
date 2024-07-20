#pragma once

#include "Kengine/types/any.hxx"

#include <unordered_map>

namespace Kengine
{
    class keyed_archive
    {
    public:
    private:
        std::unordered_map<std::string, any> table;
    };
} // namespace Kengine