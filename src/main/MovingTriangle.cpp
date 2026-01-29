#include <engine/engine.hpp>
#include <iostream>
#include <vector>

/** 
 * @struct Cofig
 * @brief Configuración y estado del proyecto
 */
struct Config { 
    // ========== CONFIGURACIÓN (valores que rara vez cambian) ========== 
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEINGHT = 800;
    const char* WINDOW_TITLE = "Moving Triangle";
    const char* VERTEX_PATH = "../../assets/shaders/MovingTriangle/VertexShader.vert";
    const char* FRAGMENT_PATH = "../../assets/shaders/MovingTriangle/FragmentShader.frag";

    // ========== ESTADO RUNTIME (valores que cambian frecuentemente) ==========
    GLFWwindow* window = nullptr;
    GLuint VAO, VBO;
    std::vector<engine::core::Vertex> triangle = {
        {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
    };
    float xOffset = 0.0f;
    float yOffset = 0.0f;
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

    glBindVertexArray(config.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 config.triangle.size() * sizeof(engine::core::Vertex),
                 config.triangle.data(), 
                 GL_DYNAMIC_DRAW);

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

    glBindVertexArray(0);
}

// ============================================================================
// FUNCIONES DE RENDERIZADO
// ============================================================================

/**
 * @brief Dibuja los ejes coordenados en la pantalla
 * 
 * Renderiza los ejes X e Y usando el modo GL_LINES.
 * 
 * @param program Programa de shaders a utilizar para el renderizado
 */
void drawTriangle(Config& config, engine::graphics::Shader& shader) {
    shader.use();
    glBindVertexArray(config.VAO);
    glDrawArrays(GL_TRIANGLES, 0, config.triangle.size());
    glBindVertexArray(0);
}

void moving(Config &config, engine::graphics::Shader &shader) {
    bool left = glfwGetKey(config.window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right = glfwGetKey(config.window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    bool up = glfwGetKey(config.window, GLFW_KEY_UP) == GLFW_PRESS;
    bool down = glfwGetKey(config.window, GLFW_KEY_DOWN) == GLFW_PRESS;

    if (left && right) {
        return;
    }
    else if (left && config.xOffset >= -0.5f) {
        config.xOffset -= 0.0001f;
        shader.setUniform("xOffset", config.xOffset);
    } 
    else if (right && config.xOffset <= 0.5f) {
        config.xOffset += 0.0001f;
        shader.setUniform("xOffset", config.xOffset);
    }

    if (up && down) {
        return;
    } 
    else if (up && config.yOffset <= 0.5) {
        config.yOffset += 0.0001f;
        shader.setUniform("yOffset", config.yOffset);
    }
    else if (down && config.yOffset >= -0.5f) {
        config.yOffset -= 0.0001f;
        shader.setUniform("yOffset", config.yOffset);
    }
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

/**
 * @brief Función principal del programa
 * 
 * Coordina la inicialización, ejecución y limpieza de la aplicación.
 * El flujo principal es:
 * 0. Inicializar configuración inicial
 * 1. Inicializar GLFW y crear ventana
 * 2. Inicializar GLAD
 * 3. Compilar shaders y crear programa
 * 4. Configurar buffers OpenGL
 * 5. Ejecutar loop de renderizado
 * 6. Limpiar recursos
 * 
 * @return int Código de salida (0 = éxito, -1 = error)
 */
int main() {
    // 0. Incialización de configuración inicial
    Config config;

    // 1. Inicialización de GLFW y ventana
    if (!windowInit(config)) return -1;

    // 2. Inicialización de GLAD
    if (!gladInit()) return -1;

    // 3. Compilación de shaders
    engine::graphics::Shader shader(config.VERTEX_PATH, config.FRAGMENT_PATH);

    // 4. Configuración de buffers OpenGL
    setupTriangle(config);

    // 5. Configuración inicial de OpenGL
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // Modo wireframe
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);      // Color de fondo negro


    // 6. Loop principal de renderizado
    while(!glfwWindowShouldClose(config.window)) {
        keyCallback(config.window);                    // Procesar entrada
        glClear(GL_COLOR_BUFFER_BIT);           // Limpiar buffer de color

        // update shader uniform
        moving(config, shader);

        // Renderizar escena
        drawTriangle(config, shader);               // Dibujar gráfica después

        glfwSwapBuffers(config.window);                // Intercambiar buffers
        glfwPollEvents();                       // Procesar eventos
    }

    // 7. Limpieza de recursos
    glDeleteVertexArrays(1, &config.VAO);
    glDeleteBuffers(1, &config.VBO);

    glfwTerminate();
    
    return 0;
}