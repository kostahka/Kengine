#pragma once

#include "Kengine/components/component.hxx"
#include "Kengine/serialization/serialization.hxx"

#include <vector>

namespace Kengine
{
    class prefab final : public serializable
    {
    public:
        prefab();

        template <typename T, typename... Types>
        void add_component(Types... args)
        {
            
        }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

    private:
        std::vector<component*> components;
    };
} // namespace Kengine