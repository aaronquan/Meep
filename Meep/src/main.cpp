#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "classes/shader/shader.hpp"
#include "classes/engine.hpp"
#include "classes/renderer.hpp"

#include "classes/text.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
/*
struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;*/

//bool load_characters();

//void RenderText(Shader& s, std::string text, float x, float y, 
//float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO);

int main() {

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
    
    //bool loading = load_characters();
    //if (loading) {
       // std::cout << "Loaded Characters" << std::endl;
    //}
    /*
    Shader shader("./src/shaders/text.vs", "./src/shaders/text.fs");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shader.use();
    shader.setMatrix4("projection", projection);*/

    /*
    unsigned int VAO, VBO;
    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);*/


    float x = 0.0f;
    float y = 0.0f;
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;

    Engine engine(SCR_WIDTH, SCR_HEIGHT);
    Renderer2D renderer;
    
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
        glClear(GL_COLOR_BUFFER_BIT);

        engine.step(deltaTime);
        renderer.render(engine);

        //DrawText::renderText("Hello", 100.0f, 100.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    engine.clean();
    renderer.clean();

    glfwTerminate();
}