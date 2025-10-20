#include <engine/graphics/sprite_sheet.hpp>
#include <iostream>

using namespace engine::graphics;

SpriteSheet::SpriteSheet(Texture& texture, GLuint spriteW, GLuint spriteH) 
            : texture(texture), spriteWidth(spriteW), spriteHeight(spriteH)
{
    if (spriteW == 0 || spriteH == 0) {
        std::cerr << "ERROR::SPRITE_SHEET::ZERO_DIMENSION: "
                  << "Sprite dimensions cannot be zero ("
                  << spriteW << "x" << spriteH << ")" << std::endl;
    }

    columns = texture.getWidth() / spriteW;
    rows = texture.getHeight() / spriteH;
}

glm::vec4 SpriteSheet::getSpriteUV(GLuint spriteX, GLuint spriteY) {
    if (spriteX >= columns || spriteY >= rows) {
        std::cerr << "ERROR::SPRITE_SHEET::OUT_OF_RANGE: "
                  << "Requested frame (" << spriteX << ", " << spriteY 
                  << ") but sheet only has " << columns << "x" << rows 
                  << " frames" << std::endl;

        return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    return glm::vec4(
        static_cast<float>(spriteX * spriteWidth) / texture.getWidth(),
        static_cast<float>(spriteY * spriteHeight) / texture.getHeight(),
        static_cast<float>((spriteX + 1) * spriteWidth) / texture.getWidth(),
        static_cast<float>((spriteY + 1) * spriteHeight) / texture.getHeight()
    );
}