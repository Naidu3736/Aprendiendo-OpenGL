#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#pragma once

#include <glad/glad.h>
#include "texture.hpp"
#include <glm/glm.hpp>

namespace engine {
    namespace graphics {
        class SpriteSheet {
            private:
            engine::graphics::Texture& texture;
            GLuint spriteWidth;
            GLuint spriteHeight;
            GLuint columns;
            GLuint rows;

            public:
            SpriteSheet(engine::graphics::Texture& texture, GLuint spriteW, GLuint spriteH);

            glm::vec4 getSpriteUV(GLuint spriteX, GLuint spriteY);

            inline GLuint getColumns() const { return columns; }

            inline GLuint getRows() const { return rows; }
        };
    }
}

#endif // SPRITE_SHEET_HPP