#include <engine/engine.hpp>
#include <iostream>
#include <vector>

struct Config {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;
    const char* WINDOWS_TITLE = "Cat";
    const char* FRAGMENT_PATH = "../../assets/shaders/Cat/FragmentShader.frag";
    const char* VERTEX_PATH = "../../assets/shaders/Cat/VertexShader.vert";
    const char* TEXTURE_CAT = "../../assets/textures/cat.png";
    const char* TEXTURE_CLOUD = "../../assets/textures/cloud.png";

    GLFWwindow* window = nullptr;

    const float ASPECT_RATIO = 16.0f/15.0f;

    std::vector<engine::core::Vertex> sit = {
        {{-0.5f, -0.5f*ASPECT_RATIO, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, (32.0f/48.0f)}},
        {{-0.5f,  0.5f*ASPECT_RATIO, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f*ASPECT_RATIO, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), 1.0f}},
        {{ 0.5f, -0.5f*ASPECT_RATIO, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), (32.0f/48.0f)}}
    };
    
    std::vector<engine::core::Vertex> walk = {
        {{-0.5f*ASPECT_RATIO, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, (16.0f/48.0f)}},
        {{-0.5f*ASPECT_RATIO,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, (32.0f/48.0f)}},
        {{ 0.5f*ASPECT_RATIO,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), (32.0f/48.0f)}},
        {{ 0.5f*ASPECT_RATIO, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), (16.0f/48.0f)}}
    };

    std::vector<engine::core::Vertex> run = {
        {{-0.5f*ASPECT_RATIO, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, (0.0f/48.0f)}},
        {{-0.5f*ASPECT_RATIO,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, (16.0f/48.0f)}},
        {{ 0.5f*ASPECT_RATIO,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), (16.0f/48.0f)}},
        {{ 0.5f*ASPECT_RATIO, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {(20.0f/120.0f), (0.0f/48.0f)}}
    };

    std::vector<GLuint> indexs {
        0, 1, 3,
        1, 2, 3
    };

    float offset = 0.0f;
    float direction = 1.0f;
    float offsetTexCoord = 0.0f;

    int frameCounter = 0;
    int framesPerAnimationFrame = 450;
};



void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(Config& config) {
    if (glfwGetKey(config.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(config.window, true);
    }

    bool left = glfwGetKey(config.window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right = glfwGetKey(config.window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    bool in_moving = false;

    if (left && right) {
        return;
    }

    if (left) {
        in_moving = true;
        config.direction = 1.0f;
        // if (config.offset >= -0.5f) {
            config.offset -= 0.0001f;
        // }
        // else {
        //     config.offset = -0.5f;
        // }
    }
    else if (right) {
        in_moving = true;
        config.direction = -1.0f;
        // if (config.offset <= 0.5f) {
            config.offset += 0.0001f;
        // }
        // else {
        //     config.offset = 0.5f;
        // }
    }
    
    if (in_moving) {
        if (++config.frameCounter >= config.framesPerAnimationFrame) {
            config.frameCounter = 0;
            config.offsetTexCoord += (20.0f/120.0f);
            
            if (config.offsetTexCoord > (100.0f/120.0f)) {
                config.offsetTexCoord = 0.0f;
            }
        }
    } 
    else {
        config.offsetTexCoord = 0.0f;
        config.frameCounter = 0;
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
                                     nullptr, nullptr);
    
    if (!config.window) {
        std::cerr << "ERROR::GLFW::WINDOW::FAILURE_INITIALITATION" << std::endl;
        glfwTerminate();
        return false;
    }                                 

    glfwMakeContextCurrent(config.window);
    glfwSetFramebufferSizeCallback(config.window, framebufferSizeCallback);

    return true;
}

bool gladInit() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {\
        std::cerr << "ERROR::GLAD::FAILURE_INITIALITATION" << std::endl;
        return false;
    }

    return true;
}

void transform(Config& config, engine::graphics::Shader& shader) {
    shader.setUniform("offsetTexCoord", config.offsetTexCoord);
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(config.offset, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(.5f * config.direction, .5f, 1.0f));

    shader.setUniform("model", model);
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

    engine::graphics::TextureParams catParams;
    catParams.magFilter = GL_NEAREST;
    catParams.minFilter = GL_NEAREST_MIPMAP_NEAREST;
    engine::graphics::Texture cat_sprite(config.TEXTURE_CAT, catParams);

    shader.use();
    shader.setUniform("Texture", 0);

    std::vector<engine::core::Vertex> walk = config.walk;

    engine::graphics::Mesh cat(walk, config.indexs, {&cat_sprite});

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.0f, 0.5f, 0.6f, 1.0f);

    while (!glfwWindowShouldClose(config.window)) {
        processInput(config);

        glClear(GL_COLOR_BUFFER_BIT);

        transform(config, shader);

        cat.draw(shader);

        glfwSwapBuffers(config.window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}