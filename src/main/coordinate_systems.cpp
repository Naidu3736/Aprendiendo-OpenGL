#include "engine/engine.hpp"
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
        {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

    std::vector<GLuint> indexs = {0, 1, 3, 1, 2, 3};
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool windowInit(Window &window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window.window = glfwCreateWindow(window.SCREEN_WIDTH, window.SCREEN_HEIGHT,
                                     window.WINDOW_TITLE, nullptr, nullptr);

    if (window.window == nullptr) {
        std::cerr << "ERROR::GLFW::WINDOW::FAILURE_INITIALITATION" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window.window);
    glfwSetFramebufferSizeCallback(window.window, framebufferSizeCallback);

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
    Objects obj;
    Paths paths;

    if (!windowInit(window))
        return -1;

    if (!gladInit())
        return -1;

    engine::graphics::Shader shader(paths.VERTEX_PATH, paths.FRAGMENT_PATH);

    engine::graphics::Texture texture0(paths.TEXTURE_PATH);

    std::vector<engine::graphics::Texture *> textures = {&texture0};

    engine::graphics::Mesh mesh(obj.vertexs, obj.indexs, textures);

    shader.use();
    shader.setUniform("uTexture", 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window.window)) {
        processInput(window.window);

        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-45.0f),
                            glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f),
                                      (float)window.SCREEN_WIDTH /
                                          (float)window.SCREEN_HEIGHT,
                                      0.1f, 100.0f);

        shader.setUniform("uModel", model);
        shader.setUniform("uView", view);
        shader.setUniform("uProjection", projection);

        mesh.draw(shader);

        glfwSwapBuffers(window.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}