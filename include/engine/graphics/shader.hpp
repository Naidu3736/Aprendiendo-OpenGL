/**
 * @file shader.hpp
 * @brief Clase Shader para manejo de programas de shaders en OpenGL
 * 
 * Esta clase proporciona una interfaz para cargar, compilar, enlazar y usar
 * shaders en OpenGL, así como para establecer variables uniformes de manera
 * type-safe mediante templates.
 * 
 * @author [Francisco Aparicio Martínez]
 * @version 1.0
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <type_traits>
#include <iostream> 
#include <fstream>
#include <sstream>

namespace engine {
    namespace graphics {
        
        /**
         * @class Shader
         * @brief Maneja la carga, compilación y uso de programas de shaders OpenGL
         * 
         * La clase Shader simplifica el proceso de trabajar con shaders en OpenGL
         * proporcionando métodos para cargar shaders desde archivos, compilarlos,
         * enlazarlos en un programa y establecer variables uniformes de manera segura.
         * 
         * @note Los shaders deben estar escritos en GLSL y seguir el estándar Core Profile
         */
        class Shader {
            private:
            /** @brief Identificador del programa de shaders en OpenGL */
            GLuint ID;

            public:
                /**
                 * @brief Constructor que carga y compila shaders desde archivos
                 * 
                 * Crea un programa de shaders cargando y compilando un vertex shader
                 * y un fragment shader desde las rutas especificadas.
                 * 
                 * @param vertexPath Ruta al archivo del vertex shader
                 * @param fragmentPath Ruta al archivo del fragment shader
                 * 
                 * @throws std::runtime_error Si falla la carga o compilación de los shaders
                 * 
                 * @example
                 * @code
                 * Shader shader("shaders/vertex.vert", "shaders/fragment.frag");
                 * @endcode
                 */
                Shader(const char* vertexPath, const char* fragmentPath);

                /**
                 * @brief Destructor - libera los recursos del programa de shaders
                 * 
                 * Elimina el programa de shaders de la GPU si existe.
                 * Es llamado automáticamente cuando el objeto Shader sale del scope.
                 */
                ~Shader();

                Shader(const Shader&) = delete;

                /**
                 * @brief Activa el programa de shaders para su uso
                 * 
                 * Hace que este programa de shaders sea el actualmente activo
                 * para todas las operaciones de renderizado subsiguientes.
                 * 
                 * @example
                 * @code
                 * shader.use();
                 * // Todo el renderizado usará estos shaders
                 * @endcode
                 */
                void use() const;

                /**
                 * @brief Establece el valor de una variable uniforme en el shader
                 * 
                 * Template method que permite establecer variables uniformes de manera
                 * type-safe. Soporta los siguientes tipos:
                 * - bool, int, float, double
                 * - glm::vec2, glm::vec3, glm::vec4
                 * - glm::mat2, glm::mat3, glm::mat4
                 * 
                 * @tparam T Tipo del valor uniforme (deducido automáticamente)
                 * @param name Nombre de la variable uniforme en el shader
                 * @param value Valor a asignar a la variable uniforme
                 * 
                 * @note Si la variable uniforme no existe, se muestra un error por consola
                 * 
                 * @example
                 * @code
                 * shader.setUniform("color", glm::vec3(1.0f, 0.0f, 0.0f));
                 * shader.setUniform("modelMatrix", model);
                 * shader.setUniform("brightness", 0.5f);
                 * @endcode
                 */
                template <typename T>
                auto setUniform(const char* name, T value)
                    -> std::enable_if_t<
                        std::is_same_v<T, bool> || 
                        std::is_same_v<T, int> ||
                        std::is_same_v<T, float> ||
                        std::is_same_v<T, double> ||
                        std::is_same_v<T, glm::vec2> ||
                        std::is_same_v<T, glm::vec3> ||
                        std::is_same_v<T, glm::vec4> ||
                        std::is_same_v<T, glm::mat2> ||
                        std::is_same_v<T, glm::mat3> ||
                        std::is_same_v<T, glm::mat4>, 
                        void
                    > 
                {
                    GLint location = glGetUniformLocation(ID, name);
                    if (location == -1) {
                        std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
                        return;
                    }

                    if constexpr (std::is_same_v<T, bool>) {
                        glUniform1i(location, static_cast<int>(value));
                    }
                    else if constexpr (std::is_same_v<T, int>) {
                        glUniform1i(location, value);
                    }
                    else if constexpr (std::is_same_v<T, float>) {
                        glUniform1f(location, value);
                    }
                    else if constexpr (std::is_same_v<T, double>) {
                        glUniform1d(location, value);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec2>) {
                        glUniform2f(location, value.x, value.y);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec3>) {
                        glUniform3f(location, value.x, value.y, value.z);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec4>) {
                        glUniform4f(location, value.x, value.y, value.z, value.w);
                    }
                    else if constexpr (std::is_same_v<T, glm::mat2>) {
                        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
                    }
                    else if constexpr (std::is_same_v<T, glm::mat3>) {
                        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
                    }
                    else if constexpr (std::is_same_v<T, glm::mat4>) {
                        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
                    }
                }
                
                /**
                 * @brief Devuelve el identificador del Shader
                 * 
                 * @return GLuint Idetificador único del Shader
                 */
                inline GLuint getID() const { return ID; }
        };
    }
}

#endif // SHADER_HPP