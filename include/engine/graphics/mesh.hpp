#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP    

#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

namespace engine {
    namespace graphics {
        class Geometry {
            private:
            struct Vertex{
                glm::vec3 position;
                glm::vec4 color;
            };
            

            GLuint VAO, VBO, EBO;
            std::vector<Vertex> vertexs;
            std::vector<GLuint> ìndexs;
            GLenum drawMode;

            public:
            Geometry(const std::vector<Vertex>& vertexs,
                    const std::vector<GLuint>& indexs) {}
        };
    }
}

#endif // GEOMETRY_HPP  