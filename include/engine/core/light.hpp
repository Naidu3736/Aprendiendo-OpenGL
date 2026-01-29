#ifndef LIGHT_HPP
#define LIGHT_HPP

#pragma once

#include <glm/glm.hpp>

namespace engine::core {
    struct Light {
        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
}

#endif // LIGHT_HPP