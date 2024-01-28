#include "Kengine/resources/sprite-material-resource.hxx"

#include "../graphics/graphics.hxx"
#include "Kengine/resources/resource-manager.hxx"

namespace Kengine
{
    sprite_material_resource::sprite_material_resource(std::string_view name)
        : material_resource(graphics::sprite_shader, name)
    {
        resource::r_type = resource_type::sprite_material;
    }

    sprite_material_resource::sprite_material_resource(
        const res_ptr<fragment_shader_res>& fragment_shader,
        std::string_view                    shader_name,
        std::string_view                    name)
        : material_resource(
              make_resource<shader_res>(
                  graphics::sprite_vertex_shader, fragment_shader, shader_name),
              name)
    {
    }

    sprite_material_resource::~sprite_material_resource() {}
} // namespace Kengine