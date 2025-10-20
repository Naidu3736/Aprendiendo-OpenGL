#include <engine/engine.hpp>
#include <vector>
#include <iostream>

struct Config {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;
    const char* WINDOWS_TITLE = "Transform";
    const char* FRAGMENT_PATH = "../assets/shaders/Transforms/FragmentShader.frag";
    const char* VERTEX_PATH = "../assets/shaders/Transforms/VertexShader.vert";
    const char* TEXTURE_1_PATH = "../assets/textures/ellen_joe.png";
    const char* TEXTURE_2_PATH = "../assets/textures/coca.png";

    GLFWwindow* window = nullptr;

    GLuint VAO, VBO, EBO;

    std::vector<engine::core::Vertex> container = {
        // Triángulo 1
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f
        }}
    };

    std::vector<GLuint> indexs = {
        0, 1, 3,
        1, 2, 3
    };

    float angle = 0.0f;
};


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

bool windowInit(Config& config) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    config.window = glfwCreateWindow(config.SCREEN_WIDTH,
        config.SCREEN_HEIGHT,
        config.WINDOWS_TITLE,
        nullptr,
        nullptr);

    if (config.window == nullptr) {
        std::cerr << "ERROR::GLFW::INITIALITATION_FAILED_WINDOW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(config.window);
    glfwSetFramebufferSizeCallback(config.window, framebufferSizeCallback);

    return true;
}

bool gladInit() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR::GLAD::INITIALITAION_FAILED" << std::endl;
        return false;
    }

    return true;
}

void setupContainer(Config& config) {
    glGenVertexArrays(1, &config.VAO);
    glGenBuffers(1, &config.VBO);
    glGenBuffers(1, &config.EBO);
    
    glBindVertexArray(config.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 config.container.size() * sizeof(engine::core::Vertex), 
                 config.container.data(), 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 config.indexs.size() * sizeof(GLuint),
                 config.indexs.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                         sizeof(engine::core::Vertex),
                        (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                         sizeof(engine::core::Vertex),
                         (void*)offsetof(engine::core::Vertex, color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                         sizeof(engine::core::Vertex),
                         (void*)offsetof(engine::core::Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void drawContainer1(Config& config, engine::graphics::Shader& shader, engine::graphics::Texture& texture) {
    shader.use();
    texture.bind(GL_TEXTURE);

    glBindVertexArray(config.VAO);

    glm::mat4 model =  glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, -0.5f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    shader.setUniform("model", model);

    glDrawElements(GL_TRIANGLES, config.indexs.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void drawContainer2(Config& config, engine::graphics::Shader& shader, engine::graphics::Texture& texture) {
    shader.use();
    texture.bind(GL_TEXTURE);

    glBindVertexArray(config.VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scale = (std::sin((float)glfwGetTime()) + 1.0f) * 0.5f;
    model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

    shader.setUniform("model", model);

    glDrawElements(GL_TRIANGLES, config.indexs.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

int main() {
    Config config;
    if (!windowInit(config)) {
        return -1;
    }

    if (!gladInit()) {
        return -1;
    }

    engine::graphics::Shader shader(config.VERTEX_PATH, config.FRAGMENT_PATH);

    engine::graphics::Texture texture1(config.TEXTURE_1_PATH);
    engine::graphics::Texture texture2(config.TEXTURE_2_PATH);

    setupContainer(config);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!glfwWindowShouldClose(config.window)) {
        processInput(config.window);

        glClear(GL_COLOR_BUFFER_BIT);

        drawContainer1(config, shader, texture1);
        drawContainer2(config, shader, texture2);

        config.angle += 0.01;
        if (config.angle >= 360) config.angle = 0.0f;

        glfwSwapBuffers(config.window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &config.VAO);
    glDeleteBuffers(1, &config.VBO);
    glDeleteBuffers(1, &config.EBO);
    glfwTerminate();

    return 0;
}