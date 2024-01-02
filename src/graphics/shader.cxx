#include "Kengine/graphics/shader.hxx"

#include "../opengl/opengl.hxx"
#include "Kengine/io/file-manager.hxx"

#include "glm/gtc/type_ptr.hpp"

#include <sstream>

namespace Kengine::graphics
{
    shader::shader(const res_ptr<shader_res>& shader_resource)
        : resource(shader_resource)
    {
        if (resource)
            resource->take_data();
    }

    shader::shader(const shader& other)
    {
        resource = other.resource;
        if (resource)
            resource->take_data();
    }

    shader::~shader()
    {
        if (resource)
            resource->free_data();
    }

    shader& shader::operator=(const shader& other)
    {
        if (resource)
            resource->free_data();

        resource = other.resource;
        if (resource)
            resource->take_data();

        return *this;
    }

    void shader::use()
    {
        KENGINE_GL_CHECK(glUseProgram(resource->get_id()));
    }

    void shader::save_uniform_location(std::string_view name)
    {
        uniform_locations[name] = KENGINE_GL_CHECK(
            glGetUniformLocation(resource->get_id(), name.data()));
    }

    uint32_t shader::get_uniform_location(std::string_view name)
    {
        KENGINE_ASSERT(uniform_locations.contains(name), "No such uniform");
        return uniform_locations[name];
    }
} // namespace Kengine::graphics