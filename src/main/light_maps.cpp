#include "engine/engine.hpp"
#include "editor/material_editor.hpp"
#include "engine/core/light.hpp"
#include "engine/core/material.hpp"
#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>

struct Window {
    GLuint SCREEN_WIDTH = 800;
    GLuint SCREEN_HEIGHT = 800;
    const char *WINDOW_TITLE = "Materials | FPS: ";
    GLFWwindow *window = nullptr;
};

struct Paths {
    const char* LIGHTING_PATH = "../../assets/shaders/light_maps/lighting.frag";
    const char* LIGHT_CUBE_PATH =  "../../assets/shaders/light_maps/light_cube.frag";
    const char* VERTEX_PATH = "../../assets/shaders/light_maps/vertex_shader.vert";
    const char* DIFFUSE_MAP_PATH = "../../assets/textures/light_maps/container2.png";
    const char* SPECULAR_MAP_PATH = "../../assets/textures/light_maps/container2_specular.png";
};

struct Cube {
    std::vector<engine::core::Vertex> vertexs = {
        // Cara frontal (z = -0.5) - Normal: (0, 0, -1)
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},   // 0 - Esquina superior derecha
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}},  // 1 - Esquina inferior derecha
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}}, // 2 - Esquina inferior izquierda
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}},  // 3 - Esquina superior izquierda

        // Cara trasera (z = 0.5) - Normal: (0, 0, 1)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},   // 4 - Esquina superior derecha
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},  // 5 - Esquina inferior derecha
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // 6 - Esquina inferior izquierda
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},  // 7 - Esquina superior izquierda

        // Cara derecha (x = 0.5) - Normal: (1, 0, 0)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},   // 8 - Esquina superior trasera
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // 9 - Esquina inferior trasera
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // 10 - Esquina inferior frontal
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},  // 11 - Esquina superior frontal

        // Cara izquierda (x = -0.5) - Normal: (-1, 0, 0)
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},  // 12 - Esquina superior frontal
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}}, // 13 - Esquina inferior frontal
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}},  // 14 - Esquina inferior trasera
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}},   // 15 - Esquina superior trasera

        // Cara superior (y = 0.5) - Normal: (0, 1, 0)
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},   // 16 - Esquina trasera derecha
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // 17 - Esquina frontal derecha
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // 18 - Esquina frontal izquierda
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},  // 19 - Esquina trasera izquierda

        // Cara inferior (y = -0.5) - Normal: (0, -1, 0)
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}}, // 20 - Esquina frontal derecha
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},  // 21 - Esquina trasera derecha
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, // 22 - Esquina trasera izquierda
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}} // 23 - Esquina frontal izquierda
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

    static bool cursorDisabled = true;
    static bool altKeyPressed = false;

    bool altCurrentlyPressed = (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS);
    if (altCurrentlyPressed && !altKeyPressed) {
        cursorDisabled = !cursorDisabled;
        
        if (cursorDisabled) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            engine::input::Mouse::reset(width, height);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    altKeyPressed = altCurrentlyPressed;

    if (cursorDisabled){
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
}

bool windowInit(Window &window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.window = glfwCreateWindow(window.SCREEN_WIDTH, window.SCREEN_HEIGHT,
                                     std::format("{}{}", window.WINDOW_TITLE, 0).c_str(), 
                                     nullptr, nullptr);

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
    Cube cube;
    
    engine::core::Light light;
    light.position = glm::vec3(1.2f, 1.0f, 2.0f);
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    engine::graphics::Camera camera;
    engine::input::Mouse::init(window.SCREEN_WIDTH, window.SCREEN_HEIGHT);
    engine::core::Timer::initialitation();  

    if (!windowInit(window) | !gladInit()) {
        return -1;
    }

    engine::graphics::Shader lighting(paths.VERTEX_PATH, paths.LIGHTING_PATH);
    engine::graphics::Shader lightCube(paths.VERTEX_PATH, paths.LIGHT_CUBE_PATH);
    engine::graphics::Texture diffuseMap(paths.DIFFUSE_MAP_PATH);
    engine::graphics::Texture specularMap(paths.SPECULAR_MAP_PATH);
    engine::graphics::Mesh object(cube.vertexs, cube.indexs, {&diffuseMap, &specularMap}, 
                                  engine::graphics::VertexAttributes::POSITION
                                | engine::graphics::VertexAttributes::NORMAL 
                                | engine::graphics::VertexAttributes::TEXCOORDS);
        
    engine::graphics::Mesh lightMesh(cube.vertexs, cube.indexs, {},
                                     engine::graphics::VertexAttributes::POSITION | 
                                     engine::graphics::VertexAttributes::NORMAL);

    lighting.use();
    lighting.setUniform("uMaterial.diffuse", 0);
    lighting.setUniform("uMaterial.specular", 1);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    double lastTime = engine::core::Timer::getTimeSinceStart();
    while (!glfwWindowShouldClose(window.window)) {
        double currentTime = engine::core::Timer::getTimeSinceStart();
        float fps = engine::core::Timer::getFPS();
        if (currentTime - lastTime >= 1.0) {
            glfwSetWindowTitle(window.window,
                               std::format("{}{}", window.WINDOW_TITLE, engine::core::Timer::getFPS()).c_str());

            lastTime = currentTime;    
        }

        engine::core::Timer::update();
        processInput(window.window, camera);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float time = static_cast<float>(glfwGetTime());
        float radius = 2.0f;
        float theta = time * 0.135f * std::sqrt(2.0);
        float phi = time * 0.3f * glm::pi<float>();

        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix((float)window.SCREEN_WIDTH / (float)window.SCREEN_HEIGHT);
        glm::mat4 model(1.0f);

        glm::mat3 normalMatrix = glm::mat3(view * model);

        lighting.use();
        lighting.setUniform("uView", view);
        lighting.setUniform("uProjection", projection);
        lighting.setUniform("uNormalMatrix", normalMatrix);
        lighting.setUniform("uMaterial.shininess", 64.0f);
        lighting.setUniform("uLight.position", light.position);
        lighting.setUniform("uLight.ambient", light.ambient);
        lighting.setUniform("uLight.diffuse", light.diffuse);
        lighting.setUniform("uLight.specular", light.specular);

        model = glm::mat4(1.0f);
        lighting.setUniform("uModel", model);

        object.draw(lighting);

        lightCube.use();
        lightCube.setUniform("uView", view);
        lightCube.setUniform("uProjection", projection);
        lightCube.setUniform("uLightColor", light.diffuse);
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, light.position);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCube.setUniform("uModel", model);
        lightMesh.draw(lightCube);

        engine::input::Mouse::update();

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}