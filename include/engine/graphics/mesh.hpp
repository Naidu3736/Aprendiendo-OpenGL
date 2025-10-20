#ifndef MEHS_HPP
#define MESH_HPP

#pragma once

#include <glad/glad.h>
#include "./shader.hpp"

namespace engine {
    namespace graphics {
        class Mesh {
            private:
            GLuint VAO;
            GLuint VBO;
            GLuint EBO;

            public:
            Mesh();
        };
    }
}

#endif // MESH_HPP