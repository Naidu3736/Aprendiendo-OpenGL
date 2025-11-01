/**
 * @file mesh.hpp
 * @brief Clase Mesh para manejo de mallas geométricas en OpenGL
 * 
 * Esta clase proporciona una interfaz para crear, gestionar y renderizar
 * mallas 3D compuestas por vértices, índices y texturas. Se encarga de
 * la gestión de buffers de vértices (VBO), elementos (EBO) y arrays de
 * vértices (VAO) en OpenGL.
 * 
 * @author [Francisco Aparicio Martínez]
 * @version 2.0
 */

#ifndef MESH_HPP
#define MESH_HPP

#pragma once

#include <glad/glad.h>
#include <vector>
#include "engine/graphics/shader.hpp"
#include "engine/graphics/texture.hpp"
#include "engine/core/vertex.hpp"

namespace engine::graphics 
{    
    /**
     * @enum VertexAttributes
     * @brief Atributos de vértice soportados por la malla
     * 
     * Define los diferentes tipos de atributos que pueden estar presentes
     * en los vértices de la malla. Se utiliza como máscara de bits para
     * configurar automáticamente los vertex attributes en OpenGL.
     */
    enum class VertexAttributes {
        POSITION    = 1 << 0,  /**< Atributo de posición 3D */
        COLOR       = 1 << 1,  /**< Atributo de color RGBA */
        TEXCOORDS   = 1 << 2,  /**< Atributo de coordenadas de textura */
        NORMAL      = 1 << 3,  /**< Atributo de vector normal */
    };

    /**
     * @brief Operador OR para combinar flags de VertexAttributes
     * 
     * Permite combinar múltiples atributos usando el operador | para
     * crear una máscara de bits que representa varios atributos.
     * 
     * @param a Primer atributo a combinar
     * @param b Segundo atributo a combinar
     * @return VertexAttributes Máscara de bits con ambos atributos
     * 
     * @example
     * @code
     * auto attributes = VertexAttributes::POSITION | VertexAttributes::NORMAL;
     * @endcode
     */
    inline VertexAttributes operator|(VertexAttributes a, VertexAttributes b)
    {
        return static_cast<VertexAttributes>(static_cast<int>(a) | static_cast<int>(b));
    }

    /**
     * @brief Operador AND para verificar flags de VertexAttributes
     * 
     * Permite verificar si un atributo específico está presente en
     * una máscara de bits de atributos.
     * 
     * @param a Máscara de atributos a verificar
     * @param b Atributo específico a buscar
     * @return bool true si el atributo está presente, false en caso contrario
     * 
     * @example
     * @code
     * bool hasNormals = attributes & VertexAttributes::NORMAL;
     * @endcode
     */
    inline bool operator&(VertexAttributes a, VertexAttributes b)
    {
        return static_cast<int>(a) & static_cast<int>(b);
    }

    /**
     * @class Mesh
     * @brief Maneja la creación y renderizado de mallas 3D
     * 
     * La clase Mesh simplifica el proceso de trabajar con geometría 3D en OpenGL
     * proporcionando métodos para crear mallas a partir de vértices e índices,
     * gestionar los buffers de GPU y renderizar la geometría con shaders y texturas.
     * 
     * @note La malla gestiona automáticamente los recursos de OpenGL (VAO, VBO, EBO)
     * @note La clase no es copiable para evitar problemas de gestión de recursos GPU
     */
    class Mesh 
    {
    private:
        /** @brief Vector de vértices que definen la geometría de la malla */
        std::vector<engine::core::Vertex> m_vertexs;
        
        /** @brief Vector de índices para renderizado indexado */
        std::vector<GLuint> m_indexs;
        
        /** @brief Vector de texturas aplicadas a la malla */
        std::vector<engine::graphics::Texture*> m_textures;
        
        /** @brief Vertex Array Object ID de OpenGL */
        GLuint m_VAO;
        
        /** @brief Vertex Buffer Object ID de OpenGL */
        GLuint m_VBO;
        
        /** @brief Element Buffer Object ID de OpenGL */
        GLuint m_EBO;

        /** @brief Máscara de bits que indica los atributos presentes en los vértices */
        VertexAttributes m_attributes;

        /**
         * @brief Configura los buffers de OpenGL para la malla
         * 
         * Método interno que crea y configura el VAO, VBO y EBO en la GPU
         * con los datos de vértices e índices proporcionados.
         * 
         * @throws std::runtime_error Si falla la configuración de los buffers
         */
        void setup();

        /**
         * @brief Configura el atributo de posición en el VAO
         * 
         * @param location Location del atributo en el shader
         */
        void setupPositionAttribute(GLuint location);

        /**
         * @brief Configura el atributo de color en el VAO
         * 
         * @param location Location del atributo en el shader
         */
        void setupColorAttribute(GLuint location);

        /**
         * @brief Configura el atributo de coordenadas de textura en el VAO
         * 
         * @param location Location del atributo en el shader
         */
        void setupTexCoordsAttribute(GLuint location);

        /**
         * @brief Configura el atributo de normal en el VAO
         * 
         * @param location Location del atributo en el shader
         */
        void setupNormalAttribute(GLuint location);

    public:
        /**
         * @brief Constructor que crea una malla a partir de geometría y texturas
         * 
         * Crea una nueva malla cargando los vértices, índices y texturas proporcionados
         * en los buffers de GPU correspondientes mediante setup(). Detecta automáticamente
         * los atributos presentes en los vértices.
         * 
         * @param vertexs Vector de vértices que definen la geometría
         * @param indexs Vector de índices para renderizado indexado
         * @param textures Vector de texturas a aplicar a la malla
         * 
         * @example
         * @code
         * std::vector<engine::core::Vertex> vertices = {...};
         * std::vector<GLuint> indices = {...};
         * std::vector<Texture> textures = {...};
         * Mesh mesh(vertices, indices, textures);
         * @endcode
         */
        Mesh(const std::vector<engine::core::Vertex>& vertexs, 
             const std::vector<GLuint>& indexs, 
             const std::vector<engine::graphics::Texture*>& textures);

        /**
         * @brief Constructor que crea una malla con atributos específicos
         * 
         * Crea una nueva malla especificando explícitamente qué atributos están presentes
         * en los vértices, útil cuando la detección automática no es suficiente.
         * 
         * @param vertexs Vector de vértices que definen la geometría
         * @param indexs Vector de índices para renderizado indexado
         * @param textures Vector de texturas a aplicar a la malla
         * @param attributes Máscara de bits que especifica los atributos presentes
         * 
         * @example
         * @code
         * auto attributes = VertexAttributes::POSITION | VertexAttributes::NORMAL;
         * Mesh mesh(vertices, indices, textures, attributes);
         * @endcode
         */
        Mesh(const std::vector<engine::core::Vertex>& vertexs, 
             const std::vector<GLuint>& indexs, 
             const std::vector<engine::graphics::Texture*>& textures,
             const VertexAttributes attributes);
        
        /**
         * @brief Destructor - libera los recursos de OpenGL
         * 
         * Elimina el VAO, VBO y EBO de la GPU si existen.
         * Es llamado automáticamente cuando el objeto Mesh sale del scope.
         */
        ~Mesh();

        /**
         * @brief Constructor de copia eliminado
         * 
         * Las mallas no pueden copiarse para evitar problemas
         * con la gestión de recursos de OpenGL.
         */
        Mesh(const Mesh&) = delete;

        /**
         * @brief Operador de asignación eliminado
         * 
         * Las mallas no pueden asignarse para evitar problemas
         * con la gestión de recursos de OpenGL.
         */
        Mesh& operator=(const Mesh&) = delete;

        /**
         * @brief Renderiza la malla usando el shader especificado
         * 
         * Activa el shader, bindea las texturas y dibuja la malla
         * usando renderizado indexado si hay índices disponibles,
         * o renderizado por arrays en caso contrario.
         * 
         * @param shader Shader a utilizar para el renderizado
         * 
         * @example
         * @code
         * Shader shader("vertex.glsl", "fragment.glsl");
         * Mesh mesh(...);
         * mesh.draw(shader);
         * @endcode
         */
        void draw(const engine::graphics::Shader& shader);

        /**
         * @brief Verifica si la malla tiene atributo de posición
         * 
         * @return bool true si tiene atributo de posición, false en caso contrario
         */
        bool hasPosition() const;

        /**
         * @brief Verifica si la malla tiene atributo de color
         * 
         * @return bool true si tiene atributo de color, false en caso contrario
         */
        bool hasColor() const;

        /**
         * @brief Verifica si la malla tiene atributo de coordenadas de textura
         * 
         * @return bool true si tiene atributo de texturas, false en caso contrario
         */
        bool hasTexCoords() const;

        /**
         * @brief Verifica si la malla tiene atributo de normal
         * 
         * @return bool true si tiene atributo de normal, false en caso contrario
         */
        bool hasNormal() const;
        
        /**
         * @brief Obtiene el ID del Vertex Array Object
         * 
         * @return GLuint Identificador del VAO en OpenGL
         */
        GLuint VAO() const;
        
        /**
         * @brief Obtiene el número de vértices en la malla
         * 
         * @return size_t Cantidad de vértices
         */
         size_t vertexCount() const;
         
         /**
         * @brief Obtiene el número de índices en la malla
         * 
         * @return size_t Cantidad de índices
         */
        size_t indexCount() const;
        
        /**
         * @brief Obtiene el número de texturas en la malla
         * 
         * @return size_t Cantidad de texturas
         */
        size_t textureCount() const;
    };
}

#endif // MESH_HPP