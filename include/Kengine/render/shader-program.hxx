#pragma once

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <string>

namespace Kengine
{

    class shader_program
    {
    public:
        virtual void use() = 0;

        virtual void set_uniform1f(std::string uniform_name, float value) = 0;
        virtual void set_uniform_matrix3fv(std::string      uniform_name,
                                           const glm::mat3& matrix)       = 0;
        virtual void set_uniform_matrix4fv(std::string      uniform_name,
                                           const glm::mat4& matrix)       = 0;

        virtual ~shader_program();
    };

    shader_program* create_shader_program(const std::string& vertex_path,
                                          const std::string& fragment_path);

    shader_program* create_shader_program_from_code(const char* vertex_code,
                                                    const char* fragment_code);
} // namespace Kengine
