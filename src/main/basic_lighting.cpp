#include "engine/engine.hpp"
#include <cmath>
#include <iostream>
#include <vector>

struct Window {
    GLuint SCREEN_WIDTH = 800;
    GLuint SCREEN_HEIGHT = 800;
    const char *WINDOW_TITLE = "Coordinate System";
    GLFWwindow *window = nullptr;
};

struct Paths {
    const char* LIGHTING_PATH = "../../assets/shaders/basic_lighting/fragment_shader.frag";
    const char* LIGHT_CUBE_PATH =  "../../assets/shaders/basic_lighting/light_cube.frag";
    const char* VERTEX_PATH = "../../assets/shaders/basic_lighting/vertex_shader.vert";
};

struct Objects {
std::vector<engine::core::Vertex> vertexs = {
    // Cara frontal (z = -0.5) - Normal: (0, 0, -1)
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},   // 0
    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},  // 1
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}}, // 2
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}},  // 3

    // Cara trasera (z = 0.5) - Normal: (0, 0, 1)
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},   // 4
    {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},  // 5
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}, // 6
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},  // 7

    // Cara derecha (x = 0.5) - Normal: (1, 0, 0)
    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},   // 8
    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},  // 9
    {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 10
    {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},  // 11

    // Cara izquierda (x = -0.5) - Normal: (-1, 0, 0)
    {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}},  // 12
    {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}}, // 13
    {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},  // 14
    {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}},   // 15

    // Cara superior (y = 0.5) - Normal: (0, 1, 0)
    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},   // 16
    {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},  // 17
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}}, // 18
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},  // 19

    // Cara inferior (y = -0.5) - Normal: (0, -1, 0)
    {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}}, // 20
    {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}},  // 21
    {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}}, // 22
    {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}} // 23
};

    std::vector<GLuint> indexs = {
        // Cara frontal
        0, 1, 3, 1, 2, 3,

        // Cara trasera
        4, 5, 7, 5, 6, 7,

        // Cara derecha
        8, 9, 11, 9, 10, 11,

        // Cara izquierda
        12, 13, 15, 13, 14, 15,

        // Cara superior
        16, 17, 19, 17, 18, 19,

        // Cara inferior
        20, 21, 23, 21, 22, 23};

    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    Window *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (win) {
        win->SCREEN_WIDTH = width;
        win->SCREEN_HEIGHT = height;
    }
}

void processInput(GLFWwindow *window, engine::graphics::Camera &camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    double deltaTime = engine::core::Timer::getDeltaTime();

    camera.rotate(engine::input::Mouse::positionDeltaX(),
                  engine::input::Mouse::positionDeltaY());

    camera.zoom(static_cast<float>(engine::input::Mouse::scrollDeltaY()));

    glm::vec3 forward =
        glm::normalize(glm::vec3(camera.forward().x, 0.0f, camera.forward().z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       camera.moveForward(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(-deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveUp(-deltaTime);
}

bool windowInit(Window &window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.window = glfwCreateWindow(window.SCREEN_WIDTH, window.SCREEN_HEIGHT,
                                     window.WINDOW_TITLE, nullptr, nullptr);

    if (!window.window) {
        std::cerr << "ERROR::GLFW::WINDOW::FAILURE_INITIALITATION" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(window.window, &window);
    glfwMakeContextCurrent(window.window);
    glfwSetFramebufferSizeCallback(window.window, framebufferSizeCallback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window,
                             engine::input::Mouse::cursorPositionCallback);
    glfwSetScrollCallback(window.window, engine::input::Mouse::scrollCallback);

    return true;
}

bool gladInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR::GLAD::FAILURE_INITIALITATION" << std::endl;
        return false;
    }

    return true;
}

int main()
{
    Window window;
    Paths paths;
    Objects obj;
    engine::graphics::Camera camera;
    engine::input::Mouse::init(window.SCREEN_WIDTH, window.SCREEN_HEIGHT);
    engine::core::Timer::initialitation();

    if (!windowInit(window) | !gladInit())
        return -1;

    engine::graphics::Shader lighting(paths.VERTEX_PATH, paths.LIGHTING_PATH);
    engine::graphics::Shader lightCube(paths.VERTEX_PATH, paths.LIGHT_CUBE_PATH);
    engine::graphics::Mesh object(obj.vertexs, obj.indexs, {}, 
                                  engine::graphics::VertexAttributes::POSITION | engine::graphics::VertexAttributes::NORMAL);
        
    engine::graphics::Mesh light(obj.vertexs, obj.indexs, {},
                                 engine::graphics::VertexAttributes::POSITION | engine::graphics::VertexAttributes::NORMAL);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window.window)) {
        engine::core::Timer::update();
        processInput(window.window, camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix((float)window.SCREEN_WIDTH / (float)window.SCREEN_HEIGHT);

        lighting.use();
        lighting.setUniform("uView", view);
        lighting.setUniform("uProjection", projection);

        lighting.setUniform("uObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        lighting.setUniform("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        lighting.setUniform("uLightPos", obj.lightPos);
        lighting.setUniform("uViewPos", camera.position());

        glm::mat4 model = glm::mat4(1.0f);
        lighting.setUniform("uModel", model);

        object.draw(lighting);

        lightCube.use();
        lightCube.setUniform("uView", view);
        lightCube.setUniform("uProjection", projection);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, obj.lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCube.setUniform("uModel", model);
        light.draw(lightCube);

        engine::input::Mouse::update();

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}