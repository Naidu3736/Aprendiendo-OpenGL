/**
 * @file PolarFunction.cpp
 * @brief Programa de prueba para clase librería Shader utilizando a  
 *        PolarFunction como prueba
 *  
 * Este programa genera y renderiza una función polar definida por:
 * r = e^sin(θ) - 2cos(4θ)
 * utilizando OpenGL 4.6 con GLFW para la gestión de ventanas.
 * 
 * @author [Francisco Aparicio Martínez]
 * @version 1.0
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <engine/engine.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// ============================================================================
// CONSTANTES Y CONFIGURACIÓN
// ============================================================================

/** Ancho de la ventana en píxeles */
const int SCREEN_WIDTH = 800;

/** Alto de la ventana en píxeles */
const int SCREEN_HEIGHT = 800; 

/** Puntero a la ventana de GLFW */
GLFWwindow* window = nullptr;

/** Path de vertex shader */
const char* vertexPath = "../assets/shaders/polar/VertexShader.vert";

/** Path de fragment shader */
const char* fragmentPath = "../assets/shaders/polar/FragmentShader.frag";

// ============================================================================
// VARIABLES GLOBALES DE OPENGL
// ============================================================================

/** Vertex Array Object para la gráfica polar */
GLuint polarVAO;

/** Vertex Buffer Object para la gráfica polar */
GLuint polarVBO;

/** Vertex Array Object para los ejes coordenados */
GLuint axesVAO;

/** Vertex Buffer Object para los ejes coordenados */
GLuint axesVBO;

// ============================================================================
// PARÁMETROS DE LA GRÁFICA
// ============================================================================

/** Factor de escala para ajustar el tamaño de la gráfica */
float scale = 0.25;

/** Resolución de la gráfica (puntos por revolución) */
int resolutionGraph = 180;

/** Número de periodos a graficar */
int period = 1;

/** Incremento angular entre puntos consecutivos */
float delta = (float)(360 * period) / resolutionGraph;

// ============================================================================
// ESTRUCTURAS DE DATOS
// ============================================================================

/**
 * @var axes
 * @brief Vector que contiene los vértices para dibujar los ejes coordenados
 * 
 * Contiene 4 vértices que definen dos líneas:
 * - Eje X: línea verde horizontal
 * - Eje Y: línea roja vertical
 */
std::vector<engine::core::Vertex> axes = {
    {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},  // Eje X - extremo derecho
    {glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)}, // Eje X - extremo izquierdo

    {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},  // Eje Y - extremo superior
    {glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)}  // Eje Y - extremo inferior
};

/** Vector que almacena los vértices de la función polar a graficar */
std::vector<engine::core::Vertex> vertexs;

// ============================================================================
// FUNCIONES DE CALLBACK Y UTILITARIAS
// ============================================================================

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

/**
 * @brief Convierte coordenadas polares a cartesianas
 * 
 * Transforma un punto en coordenadas polares (r, θ) a coordenadas
 * cartesianas (x, y) usando las fórmulas:
 * x = r * cos(θ)
 * y = r * sin(θ)
 * 
 * @param r Radio en coordenadas polares
 * @param angle Ángulo en radianes
 * @return glm::vec2 Coordenadas cartesianas (x, y)
 */
glm::vec3 toCartesian(float r, float angle) {
    float x = r * std::cos(angle);
    float y = r * std::sin(angle);

    return glm::vec3(x, y, 0.0f);
}

/**
 * @brief Genera los puntos de la función polars
 * 
 * Calcula y almacena los vértices para la función polar definida por:
 * r(θ) = e^sin(θ) - 2cos(4θ)
 * 
 * La función genera puntos desde θ=0 hasta θ=360*period grados,
 * convirtiendo cada punto a coordenadas cartesianas y aplicando
 * el factor de escala.
 */
void generationPolarFucntion() {
    for (float tetha = 0; tetha < 360 * period; tetha += delta) {
        float radian = glm::radians<float>(tetha);  // Convertir a radianes
        
        // Calcular r según la función polar: r = e^sin(θ) - 2cos(4θ)
        // float r = std::pow(glm::e<float>(), std::sin(radian)) - 2 * std::cos(4 * radian) + std::pow(std::cos(radian / 12), 5);
        float r = std::pow(glm::e<float>(), std::sin(radian)) - 2 * std::cos(4 * radian);
        r *= scale;  // Aplicar factor de escala

        engine::core::Vertex vertex;
        vertex.position = toCartesian(r, radian);  // Convertir a cartesianas
        vertex.color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);  // Color magenta

        vertexs.push_back(vertex);
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
bool windowInit() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Función Polar", NULL, NULL);
    if (window == NULL) {
        std::cout << "Error al inicializar la ventana." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

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
 */
void setupAxes() {
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);

    glBindVertexArray(axesVAO);

    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 axes.size() * sizeof(engine::core::Vertex),
                 axes.data(), 
                 GL_STATIC_DRAW);

    // Atributo 0: Posición (vec3)
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE, 
                          sizeof(engine::core::Vertex), 
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Color (vec4) - offset = sizeof(glm::vec2) = 8 bytes
    glVertexAttribPointer(1, 4, 
                          GL_FLOAT, GL_FALSE, 
                          sizeof(engine::core::Vertex), 
                          (void*)offsetof(engine::core::Vertex, color));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/**
 * @brief Configura los buffers para la gráfica polar
 * 
 * Crea y configura el VAO y VBO para la función polar,
 * estableciendo los atributos de posición y color.
 */
void setupGraph() {
    glGenVertexArrays(1, &polarVAO);
    glGenBuffers(1, &polarVBO);

    glBindVertexArray(polarVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, polarVBO);
    glBufferData(GL_ARRAY_BUFFER, 
                 vertexs.size() * sizeof(engine::core::Vertex),
                vertexs.data(), 
                GL_STATIC_DRAW);

    // Atributo 0: Posición (vec3)
    glVertexAttribPointer(0, 3,
                          GL_FLOAT, GL_FALSE, 
                          sizeof(engine::core::Vertex), 
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Color (vec4) - offset = sizeof(glm::vec2) = 8 bytes
    glVertexAttribPointer(1, 4, 
                          GL_FLOAT, GL_FALSE, 
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
 * @param shaderProgram Programa de shaders a utilizar para el renderizado
 */
void drawAxes(engine::graphics::Shader& shader) {
        shader.use();
        glBindVertexArray(axesVAO);
        glDrawArrays(GL_LINE_STRIP, 0, axes.size());  // Dibujar 2 líneas (4 vértices)
        glBindVertexArray(0);                                   
}

/**
 * @brief Dibuja la gráfica polar en la pantalla
 * 
 * Renderiza la función polar usando el modo GL_LINE_LOOP para
 * crear una línea continua y cerrada.
 * 
 * @param shaderProgram Programa de shaders a utilizar para el renderizado
 */
void drawGraph(engine::graphics::Shader &shader) {
    shader.use();
    glBindVertexArray(polarVAO);
    glDrawArrays(GL_LINE_LOOP, 0, vertexs.size());  // Línea continua cerrada
    glBindVertexArray(0);
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

/**
 * @brief Función principal del programa
 * 
 * Coordina la inicialización, ejecución y limpieza de la aplicación.
 * El flujo principal es:
 * 1. Inicializar GLFW y crear ventana
 * 2. Inicializar GLAD
 * 3. Generar datos de la función polar
 * 4. Compilar shaders y crear programa
 * 5. Configurar buffers OpenGL
 * 6. Ejecutar loop de renderizado
 * 7. Limpiar recursos
 * 
 * @return int Código de salida (0 = éxito, -1 = error)
 */
int main() {
    // 1. Inicialización de GLFW y ventana
    if (!windowInit()) return -1;

    // 2. Inicialización de GLAD
    if (!gladInit()) return -1;

    // 3. Generación de datos de la función polar
    generationPolarFucntion();

    // 4. Creación de Shader
    engine::graphics::Shader shader(vertexPath, fragmentPath);
    // 5. Configuración de buffers OpenGL
    setupAxes();
    setupGraph();

    // 6. Configuración inicial de OpenGL
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Modo wireframe
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);      // Color de fondo negro

    // Debug: mostrar puntos generados
    // GLuint i = 0;
    // for (const auto& vertex : vertexs) {
    //     std::cout << i++ << " -> {" << vertex.position.x << ", " << vertex.position.y << "}" << std::endl;
    // }

    // 7. Loop principal de renderizado
    while(!glfwWindowShouldClose(window)) {
        keyCallback(window);                    // Procesar entrada
        glClear(GL_COLOR_BUFFER_BIT);           // Limpiar buffer de color

        // Renderizar escena
        drawAxes(shader);                // Dibujar ejes primero
        drawGraph(shader);               // Dibujar gráfica después

        glfwSwapBuffers(window);                // Intercambiar buffers
        glfwPollEvents();                       // Procesar eventos
    }

    // 8. Limpieza de recursos
    glDeleteVertexArrays(1, &polarVAO);
    glDeleteBuffers(1, &polarVBO);
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);

    glfwTerminate();

    return 0;
}