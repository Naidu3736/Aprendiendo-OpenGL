#ifndef SHADER_HPP
#define SHADER_HPP

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <type_traits>
#include <iostream> 
#include <fstream>
#include <sstream>

class Shader {
    public:
    GLuint ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    ~Shader();

    void use() const;

    template <typename T>
    auto setUniform(const std::string &name, T value)
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
        GLint location = glGetUniformLocation(ID, name.c_str());
        if (location == -1) {
            std::cerr << "ERROR::SHADER::UNIFORM_NOT_FOUND: " << name << std::endl;
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
            glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat3>) {
            glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
        }
        else if constexpr (std::is_same_v<T, glm::mat4>) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
        }
    }
};

#endif // SHADER_HPP