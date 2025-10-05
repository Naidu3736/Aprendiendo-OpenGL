#include <utils/shader.hpp>
#include <filesystem>

namespace {
    std::string loadShaderFromFile(const char* shaderPath) {
        std::string shaderCode;
        std::ifstream shaderFile;

        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try {
            shaderFile.open(shaderPath);
            std::stringstream shaderStream;

            shaderStream << shaderFile.rdbuf();

            shaderFile.close();

            shaderCode = shaderStream.str();
        } catch (std::ifstream::failure &e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCEFULLY_READ" << std::endl;
        }

        return shaderCode;
    }

    GLuint compileShader(const char* shaderCode, GLenum shaderType, const std::string &typeName) {
        int sucess;
        char infolog[512];

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
        if (!sucess) {
            glGetShaderInfoLog(shader, 512, NULL, infolog);
            std::cerr << "ERROR::SHADER::" << typeName << "COMPILATION_FAILED\n" << infolog << std::endl;
            glDeleteShader(shader);
            
            return 0;
        }

        return shader;
    }

    GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
        if (vertexShader == 0 || fragmentShader == 0) {
            return 0;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        int sucess;
        char infolog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &sucess);
        if (!sucess) {
            glGetProgramInfoLog(program, 512, NULL, infolog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infolog << std::endl;
            glDeleteProgram(program);
            return 0;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);


        return program;
    }
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode = loadShaderFromFile(vertexPath);
    std::string fragmentCode = loadShaderFromFile(fragmentPath);


    GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER, "VERTEX");
    GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, "FRAGMENT");

    ID = createShaderProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (ID != 0) {
        glDeleteProgram(ID);
    }
}

void Shader::use() const {
    glUseProgram(ID);
}