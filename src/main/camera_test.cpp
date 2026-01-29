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
    const char *FRAGMENT_PATH = "../../assets/shaders/coordinate_systems/fragment_shader.frag";
    const char *VERTEX_PATH = "../../assets/shaders/coordinate_systems/vertex_shader.vert";
    const char *TEXTURE_PATH = "../../assets/textures/ellen_joe.png";
};

struct Objects {
    std::vector<engine::core::Vertex> vertexs = {
        // Cara frontal (z = -0.5)
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 0
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 1
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 2
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},  // 3

        // Cara trasera (z = 0.5)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},   // 4
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},  // 5
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 6
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // 7

        // Cara derecha (x = 0.5)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 8
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 9
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 10
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},  // 11

        // Cara izquierda (x = -0.5)
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // 12
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}}, // 13
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},  // 14
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},   // 15

        // Cara superior (y = 0.5)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},   // 16
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 17
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 18
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},  // 19

        // Cara inferior (y = -0.5)
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // 20
        {{0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},  // 21
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}, // 22
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}} // 23
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

    std::vector<glm::vec3> cubePosition = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
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

    camera.rotate(engine::input::Mouse::positionDeltaX() * deltaTime,
                  engine::input::Mouse::positionDeltaY() * deltaTime);

    camera.zoom(static_cast<float>(engine::input::Mouse::scrollDeltaY()) *
                deltaTime);

    glm::vec3 forward =
        glm::normalize(glm::vec3(camera.forward().x, 0.0f, camera.forward().z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move((float)deltaTime * forward);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(-(float)deltaTime * forward);
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     camera.moveForward(camera.movementSpeed() * deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.moveForward(-camera.movementSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(-camera.movementSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(camera.movementSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveUp(camera.movementSpeed() * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveUp(-camera.movementSpeed() * deltaTime);
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

void printVector(const glm::vec3 &v, const char *str)
{
    std::cout << str << ": (" << v.x << ", " << v.y << ", " << v.z << ")"
              << std::endl;
}

void printMatrix(const glm::mat4 &M)
{
    for (GLuint i = 0; i < 4; ++i) {
        for (GLuint j = 0; j < 4; ++j) {
            std::cout << M[i][j] << " ";
        }
        std::cout << std::endl;
    }
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

    engine::graphics::Shader shader(paths.VERTEX_PATH, paths.FRAGMENT_PATH);
    engine::graphics::Texture texture0(paths.TEXTURE_PATH);
    engine::graphics::Mesh mesh(obj.vertexs, obj.indexs, {&texture0});

    shader.use();
    shader.setUniform("uTexture", 0);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window.window)) {
        engine::core::Timer::update();
        processInput(window.window, camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.setUniform("uView", camera.getViewMatrix());

        shader.setUniform("uProjection", camera.getProjectionMatrix(
                                             (float)window.SCREEN_WIDTH /
                                             (float)window.SCREEN_HEIGHT));

        for (GLuint i = 0; i < obj.cubePosition.size(); ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, obj.cubePosition[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniform("uModel", model);
            mesh.draw(shader);
        }

        engine::input::Mouse::update();

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}