#include <engine/engine.hpp>
#include <vector>
#include <iostream>

/** 
 * @struct Cofig
 * @brief Configuración y estado del proyecto
 */
struct Config { 
    // ========== CONFIGURACIÓN (valores que rara vez cambian) ========== 
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEINGHT = 800;
    const char* WINDOW_TITLE = "Texture";
    const char* VERTEX_PATH = "../assets/shaders/Texture/VertexShader.vert";
    const char* FRAGMENT_PATH = "../assets/shaders/Texture/FragmentShader.frag";
    const char* TEXTURE1_PATH = "../assets/textures/ellen_joe.png";
    const char* TEXTURE2_PATH = "../assets/textures/coca.png";

    // ========== ESTADO RUNTIME (valores que cambian frecuentemente) ==========
    GLFWwindow* window = nullptr;
    GLuint VAO, VBO, EBO;

    std::vector<engine::core::Vertex> triangle = {
        // Triángulo 1
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f }}
    };

    std::vector<GLuint> indexs = {
        0, 1, 3,
        1, 2, 3 
    };

    float mix = 0.0f;
};

/**
 * @brief Callback para cuando se redimensiona la ventana
 * 
 * Esta función es llamada automáticamente por GLFW cuando el usuario
 * redimensiona la ventana. Actualiza el viewport de OpenGL para
 * mantener las proporciones correctas.
 * 
 * @param window Puntero a la ventana de GLFW
 * @param width Nuevo ancho de la ventana en píxeles
 * @param height Nuevo alto de la ventana en píxeles
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

/**
 * @brief Procesa la entrada del teclado
 * 
 * Verifica si se ha presionado la tecla ESC para cerrar la aplicación.
 * 
 * @param window Puntero a la ventana de GLFW
 */
void keyCallback(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

// ============================================================================
// INICIALIZACIÓN DE OPENGL Y GLFW
// ============================================================================

/**
 * @brief Inicializa GLFW y crea la ventana
 * 
 * Configura GLFW y crea una ventana con el contexto OpenGL 4.3 Core Profile.
 * 
 * @return true si la inicialización fue exitosa, false en caso contrario
 */
bool windowInit(Config& config) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    config.window = glfwCreateWindow(config.SCREEN_WIDTH, 
                                     config.SCREEN_HEINGHT, 
                                     config.WINDOW_TITLE, 
                                     nullptr, 
                                     nullptr);
    if (config.window == NULL) {
        std::cout << "Error al inicializar la ventana." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(config.window);
    glfwSetFramebufferSizeCallback(config.window, framebufferSizeCallback);

    return true;
}

/**
 * @brief Inicializa GLAD para cargar las funciones de OpenGL
 * 
 * GLAD es un cargador de funciones de OpenGL que permite acceder
 * a las funciones modernas de OpenGL.
 * 
 * @return true si la inicialización fue exitosa, false en caso contrario
 */
bool gladInit() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error al cargar glad." << std::endl;
        return false;
    }

    return true;
}

// ============================================================================
// CONFIGURACIÓN DE BUFFERS Y VERTEX ARRAYS
// ============================================================================

/**
 * @brief Configura los buffers para los ejes coordenados
 * 
 * Crea y configura el VAO y VBO para los ejes coordenados,
 * estableciendo los atributos de posición y color.
 * 
 * @param config 
 */
void setupTriangle(Config& config) {
    glGenVertexArrays(1, &config.VAO);
    glGenBuffers(1, &config.VBO);
    glGenBuffers(1, &config.EBO);

    glBindVertexArray(config.VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 config.triangle.size() * sizeof(engine::core::Vertex),
                 config.triangle.data(), 
                 GL_DYNAMIC_DRAW);
        
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    config.indexs.size() * sizeof(GLuint),
                    config.indexs.data(),
                    GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3, 
                          GL_FLOAT, 
                          GL_FALSE,
                          sizeof(engine::core::Vertex), 
                          (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(engine::core::Vertex),
                          (void*)offsetof(engine::core::Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(engine::core::Vertex),
                          (void*)offsetof(engine::core::Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

/**
 * @brief Dibuja los ejes coordenados en la pantalla
 * 
 * Renderiza los ejes X e Y usando el modo GL_LINES.
 * 
 * @param program Programa de shaders a utilizar para el renderizado
 */
void drawTriangle(Config& config, engine::graphics::Shader& shader, engine::graphics::Texture& texture1, engine::graphics::Texture& texture2) {
    shader.use();
    texture1.bind(GL_TEXTURE0);
    texture2.bind(GL_TEXTURE1);
    glBindVertexArray(config.VAO);
    // glDrawArrays(GL_TRIANGLES, 0, config.triangle.size());
    glDrawElements(GL_TRIANGLES, config.indexs.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

/**
 * @brief Cambia la visibilidad de texturas
 * 
 * Renderiza la mezcla entre la textura 1 y 2 para que se vea más o menos una textura
 * 
 * @param program Programa de shaders a utilizar para el renderizado
 */
void mixing(Config &config, engine::graphics::Shader& shader) {
    bool up = glfwGetKey(config.window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down = glfwGetKey(config.window, GLFW_KEY_DOWN) == GLFW_PRESS;

    if (up && down) {
        return;
    } 
    else if (up) {
        if (config.mix < 0.99f) {  // Margen de tolerancia
            config.mix += 0.01f;
        } else {
            config.mix = 1.0f;  // Forzar el valor exacto
        }
        shader.setUniform("uMix", config.mix);
    }
    else if (down) {
        if (config.mix > 0.01f) {  // Margen de tolerancia
            config.mix -= 0.01f;
        } else {
            config.mix = 0.0f;  // Forzar el valor exacto
        }
        shader.setUniform("uMix", config.mix);
    }
}

int main() {
    Config config;

    if (!windowInit(config)) return -1;

    if (!gladInit()) return -1;

    engine::graphics::Shader shader(config.VERTEX_PATH, config.FRAGMENT_PATH);
    engine::graphics::Texture texture1(config.TEXTURE1_PATH);
    engine::graphics::Texture texture2(config.TEXTURE2_PATH);
    
    setupTriangle(config);

    shader.use();
    shader.setUniform("texture1", 0);
    shader.setUniform("texture2", 1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(config.window)) {
        keyCallback(config.window);

        glClear(GL_COLOR_BUFFER_BIT);

        mixing(config, shader);

        drawTriangle(config, shader, texture1, texture2);

        glfwSwapBuffers(config.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &config.VAO);
    glDeleteBuffers(1, &config.VBO);
    glDeleteBuffers(1, &config.EBO);

    glfwTerminate();

    return 0;
}