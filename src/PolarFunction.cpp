/**
 * @file PolarFunction.cpp
 * @brief Programa para visualizar una función polar usando OpenGL
 * 
 * Este programa genera y renderiza una función polar definida por:
 * r = e^sin(θ) - 2cos(4θ)
 * utilizando OpenGL 4.3 con GLFW para la gestión de ventanas.
 * 
 * @author [Francisco Aparicio Martínez]
 * @version 1.0
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
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
 * @struct Vertex
 * @brief Estructura que representa un vértice con posición y color
 * 
 * Esta estructura se utiliza para almacenar los datos de cada vértice
 * que será enviado a la GPU para su renderizado.
 */
struct Vertex {
    glm::vec2 position;  /**< Coordenadas 2D de la posición (x, y) */
    glm::vec3 color;     /**< Color RGB del vértice */
};

/**
 * @var axes
 * @brief Vector que contiene los vértices para dibujar los ejes coordenados
 * 
 * Contiene 4 vértices que definen dos líneas:
 * - Eje X: línea verde horizontal
 * - Eje Y: línea roja vertical
 */
std::vector<Vertex> axes = {
    {glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},  // Eje X - extremo derecho
    {glm::vec2(-1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)}, // Eje X - extremo izquierdo

    {glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)},  // Eje Y - extremo superior
    {glm::vec2(0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f)}  // Eje Y - extremo inferior
};

/** Vector que almacena los vértices de la función polar a graficar */
std::vector<Vertex> vertexs;

// ============================================================================
// SHADERS - CÓDIGO GLSL
// ============================================================================

/**
 * @var vertexShaderSource
 * @brief Código fuente del vertex shader en GLSL
 * 
 * Este shader se ejecuta una vez por cada vértice y se encarga de:
 * - Recibir la posición y color del vértice
 * - Transformar la posición al espacio de clip
 * - Pasar el color al fragment shader
 */
const char* vertexShaderSource = R"(
    #version 460 core

    layout (location = 0) in vec3 aPos;  // Atributo de posición
    layout (location = 1) in vec3 aColor; // Atributo de color
    out vec3 Color;                      // Color de salida al fragment shader

    void main() {
       gl_Position = vec4(aPos, 1.0f);   // Transformación a espacio de clip
       Color = aColor;                   // Pasar color al fragment shader
    }
)";

/**
 * @var fragmentShaderSource
 * @brief Código fuente del fragment shader en GLSL
 * 
 * Este shader se ejecuta una vez por cada fragmento (píxel) y se encarga de:
 * - Recibir el color interpolado del vertex shader
 * - Asignar el color final al fragmento
 */
const char* fragmentShaderSource = R"(
    #version 460 core

    in vec3 Color;                       // Color interpolado del vertex shader
    out vec4 outColor;                   // Color final de salida

    void main() {
       outColor = vec4(Color, 1.0f);     // Asignar color con alpha=1 (opaco)
    }
)";

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
glm::vec2 toCartesian(float r, float angle) {
    float x = r * std::cos(angle);
    float y = r * std::sin(angle);

    return glm::vec2(x, y);
}

/**
 * @brief Genera los puntos de la función polar
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

        Vertex vertex;
        vertex.position = toCartesian(r, radian);  // Convertir a cartesianas
        vertex.color = glm::vec3(1.0f, 0.0f, 1.0f);  // Color magenta

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
// COMPILACIÓN Y LINKEO DE SHADERS
// ============================================================================

/**
 * @brief Compila un shader individual
 * 
 * Compila el código fuente de un shader (vertex o fragment) y verifica
 * si la compilación fue exitosa.
 * 
 * @param source Código fuente del shader en GLSL
 * @param shaderType Tipo de shader (GL_VERTEX_SHADER o GL_FRAGMENT_SHADER)
 * @return GLuint Identificador del shader compilado, o 0 si falló
 */
GLuint compileShader(const char* source, GLenum shaderType) {
    int sucess;
	char infolog[512];

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cerr << "ERROR::SHADER::"
                  << (shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                  << "::COMPILATION_FAILED\n" << infolog << std::endl;
        glDeleteShader(shader);
        return 0;
	}

    return shader;
}

/**
 * @brief Crea y linkea un programa de shaders completo
 * 
 * Compila los shaders vertex y fragment, los linkea en un programa
 * y verifica que el linkeo sea exitoso.
 * 
 * @param vertexSource Código fuente del vertex shader
 * @param fragmentSource Código fuente del fragment shader
 * @return GLuint Identificador del programa de shaders, o 0 si falló
 */
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0) {
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int sucess;
    char infolog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infolog << std::endl;
        glDeleteProgram(shaderProgram);
        return 0;
	}

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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
    glBufferData(GL_ARRAY_BUFFER, axes.size() * sizeof(Vertex), axes.data(), GL_STATIC_DRAW);

    // Atributo 0: Posición (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Color (vec3) - offset = sizeof(glm::vec2) = 8 bytes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
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
    glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(Vertex), vertexs.data(), GL_STATIC_DRAW);

    // Atributo 0: Posición (vec2)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Color (vec3) - offset = sizeof(glm::vec2) = 8 bytes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
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
void drawAxes(GLuint &shaderProgram) {
        glUseProgram(shaderProgram);
        glBindVertexArray(axesVAO);
        glDrawArrays(GL_LINES, 0, axes.size());  // Dibujar 2 líneas (4 vértices)
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
void drawGraph(GLuint &shaderProgram) {
    glUseProgram(shaderProgram);
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

    // 4. Compilación de shaders
    GLuint shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

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
        drawAxes(shaderProgram);                // Dibujar ejes primero
        drawGraph(shaderProgram);               // Dibujar gráfica después

        glfwSwapBuffers(window);                // Intercambiar buffers
        glfwPollEvents();                       // Procesar eventos
    }

    // 8. Limpieza de recursos
    glDeleteVertexArrays(1, &polarVAO);
    glDeleteBuffers(1, &polarVBO);
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}