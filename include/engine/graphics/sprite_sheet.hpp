/**
 * @file sprite_sheet.hpp
 * @brief Clase SpriteSheet para manejo de hojas de sprites en OpenGL
 * 
 * Esta clase proporciona una interfaz para trabajar con hojas de sprites (sprite sheets),
 * permitiendo extraer coordenadas UV específicas para animaciones y sprites 2D.
 * 
 * @author [Tu Nombre]
 * @version 1.0
 */

#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#pragma once

#include <glad/glad.h>
#include "engine/graphics/texture.hpp"
#include <glm/glm.hpp>

namespace engine::graphics {
    
    /**
     * @class SpriteSheet
     * @brief Maneja la extracción de sprites desde una hoja de sprites
     * 
     * La clase SpriteSheet simplifica el proceso de trabajar con animaciones 2D
     * y sprites individuales desde una textura que contiene múltiples frames.
     * Calcula automáticamente las coordenadas UV para cada sprite basado en
     * las dimensiones del sprite y su posición en la hoja.
     * 
     * @note La textura debe ser cargada previamente y gestionada externamente
     * @note Las coordenadas spriteX y spriteY empiezan en (0,0) - esquina superior izquierda
     */
    class SpriteSheet {
    private:
        /** @brief Referencia a la textura que contiene la hoja de sprites */
        engine::graphics::Texture* m_texture;
        
        /** @brief Ancho de cada sprite individual en píxeles */
        GLuint m_spriteWidth;
        
        /** @brief Alto de cada sprite individual en píxeles */
        GLuint m_spriteHeight;
        
        /** @brief Número de columnas en la hoja de sprites */
        GLuint m_columns;
        
        /** @brief Número de filas en la hoja de sprites */
        GLuint m_rows;

    public:
        /**
         * @brief Constructor que crea una hoja de sprites desde una textura
         * 
         * @param texture Referencia a la textura que contiene todos los sprites
         * @param spriteW Ancho de cada sprite individual en píxeles
         * @param spriteH Alto de cada sprite individual en píxeles
         * 
         * @throws std::invalid_argument Si las dimensiones del sprite no son válidas
         * 
         * @example
         * @code
         * Texture spriteTexture("characters.png");
         * SpriteSheet characterSprites(spriteTexture, 64, 64); // Sprites 64x64
         * @endcode
         */
        SpriteSheet(engine::graphics::Texture* texture, GLuint spriteW, GLuint spriteH);
        
        /**
         * @brief Obtiene las coordenadas UV de un sprite específico
         * 
         * Calcula y retorna las coordenadas de textura (UV) para el sprite
         * en la posición (spriteX, spriteY) dentro de la hoja de sprites.
         * 
         * @param spriteX Coordenada X del sprite (0-based, de izquierda a derecha)
         * @param spriteY Coordenada Y del sprite (0-based, de arriba a abajo)
         * @return glm::vec4 Coordenadas UV en formato (u_min, v_min, u_max, v_max)
         * 
         * @example
         * @code
         * // Obtener el sprite en la columna 2, fila 1
         * glm::vec4 uv = spriteSheet.getSpriteUV(2, 1);
         * // uv contiene: (u_min, v_min, u_max, v_max)
         * @endcode
         */
        glm::vec4 spriteUV(GLuint spriteX, GLuint spriteY);
        
        /**
         * @brief Obtiene el número total de columnas en la hoja de sprites
         * 
         * @return GLuint Número de columnas
         */
        GLuint columns() const;
        
        /**
         * @brief Obtiene el número total de filas en la hoja de sprites
         * 
         * @return GLuint Número de filas
         */
        GLuint rows() const;
        
        /**
         * @brief Obtiene el ancho de cada sprite individual
         * 
         * @return GLuint Ancho del sprite en píxeles
         */
        GLuint spriteWidth() const;
        
        /**
         * @brief Obtiene el alto de cada sprite individual
         * 
         * @return GLuint Alto del sprite en píxeles
         */
        GLuint spriteHeight() const;
        
        /**
         * @brief Obtiene la textura de la hoja de sprites
         * 
         * @return Texture& Referencia a la textura
         */
        engine::graphics::Texture* texture() const;

        /**
         * @brief Verifica si la textura es valida
         * 
         * @return bool Si la textura no es nula
         */
        bool isValid() const;
    };
}

#endif // SPRITE_SHEET_HPP