#ifndef VERTEX_HPP
#define VERTEX_HPP

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace engine {
    namespace core {
        /**
         * @struct Vertex
         * @brief Estructura que representa un vértice con posición y color
         * 
         * Esta estructura se utiliza para almacenar los datos de cada vértice
         * que será enviado a la GPU para su renderizado.
         */
        struct Vertex{
            glm::vec3 position;
            glm::vec4 color;
            glm::vec2 texCoords;
            
            Vertex(glm::vec3 pos = glm::vec3(0.0f),
                glm::vec4 col = glm::vec4(1.0f),
                glm::vec2 tex = glm::vec2(0.0f))
                : position(pos), color(col), texCoords(tex) {}
        };
    }
}
#endif // VERTEX_HPP    