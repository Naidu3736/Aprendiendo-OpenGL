#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "engine/graphics/texture.hpp"

namespace engine::core {
    struct Material {
        private:
        using Texture = engine::graphics::Texture;

        public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        std::shared_ptr<Texture> diffuseMap;
        std::shared_ptr<Texture> specularMap;
    };
}

#endif // MATERIAL_HPP