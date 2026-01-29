#ifndef VERTEX_HPP
#define VERTEX_HPP

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace engine {
    namespace core {
        /**
         * @struct Vertex
         * @brief Estructura que representa un vértice con posición, color 
         *        y coordenadas de texturas.
         * 
         * Esta estructura se utiliza para almacenar los datos de cada vértice
         * que será enviado a la GPU para su renderizado.
         */
        struct Vertex{
            glm::vec3 m_position;
            glm::vec4 m_color;
            glm::vec2 m_texCoords;
            glm::vec3 m_normal;

            // Constructor por defecto
            Vertex() 
                : m_position(0.0f)
                , m_color(1.0f)
                , m_texCoords(0.0f)
                , m_normal(0.0f) {}

            // Solo posición
            Vertex(const glm::vec3& pos) 
                : m_position(pos)
                , m_color(1.0f)
                , m_texCoords(0.0f)
                , m_normal(0.0f) {}

            // Posición y color
            Vertex(const glm::vec3& pos, const glm::vec4& col) 
                : m_position(pos)
                , m_color(col)
                , m_texCoords(0.0f)
                , m_normal(0.0f) {}

            // Posición y coordenadas de textura
            Vertex(const glm::vec3& pos, const glm::vec2& tex) 
                : m_position(pos)
                , m_color(1.0f)
                , m_texCoords(tex)
                , m_normal(0.0f) {}

            // Posición y normal
            Vertex(const glm::vec3& pos, const glm::vec3& nor) 
                : m_position(pos)
                , m_color(1.0f)
                , m_texCoords(0.0f)
                , m_normal(nor) {}

            // Posición, textura y normal
            Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& nor) 
                : m_position(pos)
                , m_color(1.0f)
                , m_texCoords(tex)
                , m_normal(nor) {}

            // Constructor completo
            Vertex(const glm::vec3& pos, 
                   const glm::vec4& col, 
                   const glm::vec2& tex, 
                   const glm::vec3& nor = glm::vec3(0.0f)) 
                : m_position(pos)
                , m_color(col)
                , m_texCoords(tex)
                , m_normal(nor) {}
        };
    }
}
#endif // VERTEX_HPP    