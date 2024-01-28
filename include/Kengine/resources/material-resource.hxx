#pragma once

#include "Kengine/graphics/shader-utils/shader-type.hxx"
#include "Kengine/resources/shader-resource.hxx"
#include "Kengine/resources/texture-resource.hxx"

#include <string_view>
#include <unordered_map>
#include <vector>

namespace Kengine
{
    class material_resource : public resource
    {
    public:
        material_resource(std::string_view name);
        material_resource(const res_ptr<shader_res>&, std::string_view name);
        ~material_resource();

        void set_texture(uint32_t texture_block,
                         const res_ptr<texture_resource>&);
        void set_property(string_id name, graphics::shader_type_any value);

        graphics::shader_type_any get_property(string_id name) const;

        inline const res_ptr<shader_res>& get_shader() const { return shader; }

        inline const std::unordered_map<uint32_t, res_ptr<texture_resource>>&
        get_textures() const
        {
            return textures;
        }

        inline const std::unordered_map<string_id, graphics::shader_type_any>&
        get_properties()
        {
            return properties;
        }

        std::size_t serialize(std::ostream& os) const override;
        std::size_t deserialize(std::istream& is) override;

    protected:
        void load_data() override;
        void unload_data() override;

        std::unordered_map<string_id, graphics::shader_type_any> properties;
        std::unordered_map<uint32_t, res_ptr<texture_resource>>  textures;
        res_ptr<shader_res>                                      shader;
    };
} // namespace Kengine