#pragma once

#include "Kengine/serialization/serialization.hxx"
#include "res-ptr.hxx"

namespace Kengine
{
    class resource_link : public serializable
    {
    public:
        resource_link();

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

    private:
        res_ptr<resource> res;
        path              res_path;
    };
} // namespace Kengine