#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

//#include "shaders/shader.hpp"
#include "engine/engine.hpp"
#include "engine/renderer.hpp"

#include "text/text.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "window/window.hpp"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//Engine engine(SCR_WIDTH, SCR_HEIGHT, 100.0f, 100.0f);
//Renderer2D renderer(SCR_WIDTH, SCR_HEIGHT);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    //engine.onMouseMove(xpos, ypos);
}

int main() {
    Window window;
    window.create(AppEngine::s_window_dimensions, "Meep");
    window.useDefaultSizeCallback();

    DrawText::setup(AppEngine::s_window_dimensions.width, AppEngine::s_window_dimensions.height);

    glEnable(GL_BLEND);

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    AppEngine engine;
    window.setEngine(&engine);
    window.useMouseMoveCallback();

    AppRenderer renderer;

    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    window.loop([&]() {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        engine.update(deltaTime);
        renderer.renderEngine(engine);
    });
    /*
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    float x = 0.0f;
    float y = 0.0f;
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    
    engine.setup();
    renderer.setup(engine);
    DrawText::setup(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        const float speed = 2.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            y += speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            y -= speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            x -= speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            x += speed;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        engine.step(deltaTime);
        renderer.render(engine);

        DrawText::renderText("Hello", 100.0f, 100.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    engine.clean();
    renderer.clean();
    */
    glfwTerminate();
    
}