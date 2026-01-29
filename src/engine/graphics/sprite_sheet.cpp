#include "engine/graphics/sprite_sheet.hpp"
#include <iostream>

using namespace engine::graphics;

SpriteSheet::SpriteSheet(Texture* texture, 
                         GLuint spriteW, 
                         GLuint spriteH)
    : m_texture(texture)
    , m_spriteWidth(spriteW)
    , m_spriteHeight(spriteH)
    , m_columns(0)
    , m_rows(0)
{
    if (spriteW == 0 || spriteH == 0) {
        std::cerr << "ERROR::SPRITE_SHEET::ZERO_DIMENSION: "
                  << "Sprite dimensions cannot be zero ("
                  << spriteW << "x" << spriteH << ")" << std::endl;
        return;
    }

    if (texture == nullptr) {
        std::cerr << "ERROR::SPRITE_SHEET::NULL_TEXTURE: "
                  << "Texture pointer cannot be null" << std::endl;
        return;
    }

    m_columns = texture->width() / spriteW;
    m_rows = texture->height() / spriteH;
    
    if (m_columns == 0 || m_rows == 0) {
        std::cerr << "ERROR::SPRITE_SHEET::INVALID_DIMENSIONS: "
                  << "Sprite dimensions " << spriteW << "x" << spriteH
                  << " are larger than texture dimensions "
                  << texture->width() << "x" << texture->height() << std::endl;
    }
}

glm::vec4 SpriteSheet::spriteUV(GLuint spriteX, GLuint spriteY) 
{
    if (spriteX >= m_columns || spriteY >= m_rows) {
        std::cerr << "ERROR::SPRITE_SHEET::OUT_OF_RANGE: "
                  << "Requested frame (" << spriteX << ", " << spriteY 
                  << ") but sheet only has " << m_columns << "x" << m_rows 
                  << " frames" << std::endl;

        return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    }

    return glm::vec4(
        static_cast<float>(spriteX * m_spriteWidth) / m_texture->width(),
        static_cast<float>(spriteY * m_spriteHeight) / m_texture->height(),
        static_cast<float>((spriteX + 1) * m_spriteWidth) / m_texture->width(),
        static_cast<float>((spriteY + 1) * m_spriteHeight) / m_texture->height()
    );
}

GLuint SpriteSheet::columns() const 
{ 
    return m_columns; 
}

GLuint SpriteSheet::rows() const 
{ 
    return m_rows; 
}

GLuint SpriteSheet::spriteWidth() const 
{ 
    return m_spriteWidth; 
}

GLuint SpriteSheet::spriteHeight() const 
{ 
    return m_spriteHeight; 
}

engine::graphics::Texture* SpriteSheet::texture() const 
{ 
    return m_texture; 
}

bool SpriteSheet::isValid() const 
{
    return !m_texture;
}