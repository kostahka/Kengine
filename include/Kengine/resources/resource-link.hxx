#pragma once

#include "Kengine/serialization/serialization.hxx"
#include "res-ptr.hxx"

namespace Kengine
{
    class resource_link : public serializable
    {
    public:
        resource_link();
        resource_link(res_ptr<resource> res, path res_path);

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

        inline res_ptr<resource> get_resource() { return res; }

        inline const path& get_resource_path() const { return res_path; }

    private:
        res_ptr<resource> res;
        path              res_path;
    };
} // namespace Kengine