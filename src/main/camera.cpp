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

struct Mouse {
    float lastX;
    float lastY;
    bool first = true;
};

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 worldUp = up;

    float yaw = -90.0f;
    float pich = 0.0f;
    float roll = 0.0f;
    float fov = 120.0f;
    float speed = 2.5f;
    float sensitivity = 19.0f;

    bool boost = false;
};

struct Time {
    float currentTime = 0.0f;
    float lastTime = 0.0f;
    float deltaTime = 0.0f;
};

struct App {
    Window *window;
    Camera *camera;
    Time *time;
    Mouse *mouse;
};

void updateForward(Camera &camera)
{
    glm::vec3 direction(std::cos(glm::radians(camera.yaw)) *
                            std::cos(glm::radians(camera.pich)),
                        std::sin(glm::radians(camera.pich)),
                        std::sin(glm::radians(camera.yaw)) *
                            std::cos(glm::radians(camera.pich)));

    camera.forward = glm::normalize(direction);
    camera.right = glm::normalize(glm::cross(camera.forward, camera.worldUp));
    camera.up = glm::normalize(glm::cross(camera.right, camera.forward));

    // Debug de vectores de la cámara
    // printVector(camera.forward, "Forward");
    // printVector(camera.right, "Right");
    // printVector(camera.up, "Up");

    // Lógica de prueba de rotación roll
    // if (camera.roll != 0) {
    //     float cosRoll = std::cos(glm::radians(camera.roll));
    //     float sinRoll = std::sin(glm::radians(camera.roll));

    //     glm::vec3 newRight = cosRoll * camera.right - sinRoll * camera.up;
    //     glm::vec3 newUp = glm::cross(newRight, camera.forward);

    //     camera.right = glm::normalize(newRight);
    //     camera.up = glm::normalize(newUp);
    // }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);

    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (app && app->window) {
        app->window->SCREEN_WIDTH = width;
        app->window->SCREEN_HEIGHT = height;
    }
}

void mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    if (!app || !app->camera || !app->mouse)
        return;

    Camera &camera = *app->camera;
    Mouse &mouse = *app->mouse;
    Time &time = *app->time;

    if (mouse.first) {
        mouse.lastX = xPos;
        mouse.lastY = yPos;
        mouse.first = false;
    }

    float xOffset = xPos - mouse.lastX;
    float yOffset = mouse.lastY - yPos;

    mouse.lastX = xPos;
    mouse.lastY = yPos;

    xOffset *= camera.sensitivity * time.deltaTime;
    yOffset *= camera.sensitivity * time.deltaTime;

    camera.yaw += xOffset;
    camera.pich += yOffset;

    if (camera.pich > 89.9f)
        camera.pich = 89.9f;
    if (camera.pich < -89.9f)
        camera.pich = -89.9f;

    updateForward(camera);
}

void scrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    App *app = static_cast<App *>(glfwGetWindowUserPointer(window));
    Camera &camera = *app->camera;

    camera.fov -= (float)yOffset;
    if (camera.fov < 1.0f)
        camera.fov = 1.0f;
    if (camera.fov > 150.0f)
        camera.fov = 150.0f;
}

void processInput(GLFWwindow *window, Camera &camera, const Time &time)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.speed = static_cast<float>(2.5f * time.deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        if (!camera.boost) {
            camera.speed *= 2.0f;
            camera.fov *= 1.05f;
            camera.boost = true;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position +=
            glm::vec3(camera.forward.x, 0.0f, camera.forward.z) * camera.speed;
    else {
        if (camera.boost) {
            camera.speed = static_cast<float>(2.5f * time.deltaTime);
            camera.fov = 120.0f;
            camera.boost = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -=
            glm::vec3(camera.forward.x, 0.0f, camera.forward.z) * camera.speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= camera.right * camera.speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += camera.right * camera.speed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.position += camera.worldUp * camera.speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.position -= camera.worldUp * camera.speed;

    // Prueba de rotación roll (giro completamente libre)
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //     camera.pich += camera.sensitivity;
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //     camera.pich -= camera.sensitivity;
    // if (glfwGetKey(window, GLFW_KEY_RIGHT))
    //     camera.yaw += camera.sensitivity;
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     camera.yaw -= camera.sensitivity;
    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //     camera.roll += camera.sensitivity;
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //     camera.roll -= camera.sensitivity;

    if (camera.pich > 89.9f)
        camera.pich = 89.9f;
    if (camera.pich < -89.9f)
        camera.pich = -89.9f;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
        camera.forward = glm::vec3(0.0f, 0.0f, -1.0f);
        camera.right = glm::vec3(1.0f, 0.0f, 0.0f);
        camera.up = glm::vec3(0.0f, 1.0f, 0.0f);

        camera.yaw = -90.0f;
        camera.pich = 0.0f;
        camera.roll = 0.0f;
    }

    updateForward(camera);
}

bool windowInit(Window &window, App *app)
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

    glfwSetWindowUserPointer(window.window, app);
    glfwMakeContextCurrent(window.window);
    glfwSetFramebufferSizeCallback(window.window, framebufferSizeCallback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window, mouseCallback);
    glfwSetScrollCallback(window.window, scrollCallback);

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
    Camera camera;
    Mouse mouse = {window.SCREEN_WIDTH / 2.0f, window.SCREEN_HEIGHT / 2.0f};
    Time time;
    App app = {&window, &camera, &time, &mouse};

    if (!windowInit(window, &app) | !gladInit())
        return -1;

    engine::graphics::Shader shader(paths.VERTEX_PATH, paths.FRAGMENT_PATH);
    engine::graphics::Texture texture0(paths.TEXTURE_PATH);
    engine::graphics::Mesh mesh(obj.vertexs, obj.indexs, {&texture0});

    shader.use();
    shader.setUniform("uTexture", 0);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    while (!glfwWindowShouldClose(window.window)) {
        time.currentTime = static_cast<float>(glfwGetTime());
        time.deltaTime = time.currentTime - time.lastTime;
        time.lastTime = time.currentTime;

        processInput(window.window, camera, time);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(
            camera.position, camera.position + camera.forward, camera.up);
        shader.setUniform("uView", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov),
                                                (float)window.SCREEN_WIDTH /
                                                    (float)window.SCREEN_HEIGHT,
                                                0.1f, 100.0f);
        shader.setUniform("uProjection", projection);

        for (GLuint i = 0; i < obj.cubePosition.size(); ++i) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, obj.cubePosition[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setUniform("uModel", model);
            mesh.draw(shader);
        }

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}