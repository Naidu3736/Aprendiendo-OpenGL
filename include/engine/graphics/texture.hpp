/**
 * @file texture.hpp
 * @brief Clase Texture para manejo de texturas en OpenGL
 * 
 * Esta clase proporciona una interfaz para cargar, configurar y usar
 * texturas en OpenGL, con soporte para diferentes formatos de imagen
 * y parámetros de filtrado y wrapping.
 * 
 * @author [Francisco Aparicio Martínez]
 * @version 1.0
 */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP 

#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>

namespace engine {
    namespace graphics {
        
        /**
         * @struct TextureParams
         * @brief Parámetros de configuración para texturas OpenGL
         * 
         * Esta estructura agrupa todos los parámetros configurables de una textura
         * permitiendo una inicialización más legible y mantenible mediante
         * named parameters idiom.
         * 
         * @example
         * @code
         * TextureParams params;
         * params.wrapS = GL_CLAMP_TO_EDGE;
         * params.wrapT = GL_CLAMP_TO_EDGE;
         * params.magFilter = GL_NEAREST;
         * Texture texture("character.png", params);
         * 
         * // O inline:
         * Texture texture("wall.jpg", {GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR, GL_NEAREST});
         * @endcode
         */
        struct TextureParams {
            /** 
             * @brief Modo de wrapping en el eje S (coordenada U)
             * 
             * Controla cómo se muestran los texels fuera del rango [0,1] en el eje horizontal.
             * Valores comunes: GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
             * 
             * @default GL_REPEAT
             */
            GLenum wrapS = GL_REPEAT;
            
            /** 
             * @brief Modo de wrapping en el eje T (coordenada V)
             * 
             * Controla cómo se muestran los texels fuera del rango [0,1] en el eje vertical.
             * Mismos valores disponibles que wrapS.
             * 
             * @default GL_REPEAT
             */
            GLenum wrapT = GL_REPEAT;
            
            /** 
             * @brief Filtro de minimización
             * 
             * Controla cómo se muestran los texels cuando la textura se ve más pequeña
             * que su resolución original (minification).
             * Valores comunes: GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR
             * 
             * @default GL_LINEAR_MIPMAP_LINEAR
             */
            GLenum minFilter = GL_LINEAR_MIPMAP_LINEAR;
            
            /** 
             * @brief Filtro de magnificación
             * 
             * Controla cómo se muestran los texels cuando la textura se ve más grande
             * que su resolución original (magnification).
             * Valores comunes: GL_NEAREST, GL_LINEAR
             * 
             * @note No se usan mipmaps para magnificación
             * @default GL_LINEAR
             */
            GLenum magFilter = GL_LINEAR;
        };
        
        /**
         * @class Texture
         * @brief Maneja la carga, configuración y uso de texturas OpenGL
         * 
         * La clase Texture simplifica el proceso de trabajar con texturas en OpenGL
         * proporcionando métodos para cargar texturas desde archivos, configurar
         * parámetros de filtrado y wrapping, y activarlas para renderizado.
         * 
         * @note Las texturas deben estar en formatos soportados por stb_image
         * @note La clase no es copiable para evitar problemas de gestión de recursos
         */
        class Texture {
        private:
            /** @brief Identificador de la textura en OpenGL */
            GLuint ID;
            /** @brief Ancho de la textura en píxeles */
            int width;
            /** @brief Alto de la textura en píxeles */
            int height;
            /** @brief Número de canales de color (RGB=3, RGBA=4) */
            int nrChannels;

        public:
            /**
             * @brief Constructor que carga y configura una textura desde archivo
             * 
             * Crea una textura cargando una imagen desde la ruta especificada
             * y configura sus parámetros de wrapping y filtrado usando la estructura TextureParams.
             * 
             * @param path Ruta al archivo de la textura
             * @param params Estructura con los parámetros de configuración de la textura
             * 
             * @throws std::runtime_error Si falla la carga de la textura
             * 
             * @example
             * @code
             * // Uso con parámetros por defecto
             * Texture texture1("assets/textures/wall.jpg");
             * 
             * // Uso con parámetros personalizados
             * TextureParams params;
             * params.wrapS = GL_CLAMP_TO_EDGE;
             * params.magFilter = GL_NEAREST;
             * Texture texture2("assets/character.png", params);
             * 
             * // Uso inline
             * Texture texture3("ui.png", {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST});
             * @endcode
             */
            Texture(const char* path, const TextureParams& params = TextureParams());
            
            /**
             * @brief Destructor - libera los recursos de la textura
             * 
             * Elimina la textura de la GPU si existe.
             * Es llamado automáticamente cuando el objeto Texture sale del scope.
             */
            ~Texture();

            /**
             * @brief Constructor de copia eliminado
             * 
             * Las texturas no pueden copiarse para evitar problemas
             * con la gestión de recursos de OpenGL.
             */
            Texture(const Texture&) = delete;

            /**
             * @brief Activa la textura para su uso en renderizado
             * 
             * Hace que esta textura sea la actualmente activa en la unidad
             * de textura especificada para todas las operaciones de renderizado subsiguientes.
             * 
             * @param textureUnit Unidad de textura a utilizar (por defecto: GL_TEXTURE0)
             * 
             * @example
             * @code
             * texture.bind(GL_TEXTURE0);
             * // La textura estará disponible en el shader como texture0
             * @endcode
             */
            void bind(GLenum textureUnit = GL_TEXTURE0) const;
            
            /**
             * @brief Obtiene el identificador de la textura en OpenGL
             * 
             * @return GLuint Identificador único de la textura
             * 
             * @note Útil para operaciones avanzadas que requieran acceso directo al ID
             * 
             * @example
             * @code
             * GLuint texID = texture.getID();
             * glBindTexture(GL_TEXTURE_2D, texID);
             * @endcode
             */
            GLuint getID() { return ID; }
        };
    }
}

#endif // TEXTURE_HPP