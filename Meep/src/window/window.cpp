#include "window.hpp"



Window::Window() : m_window(nullptr), m_engine(nullptr), m_dimensions(0, 0),
m_current_frame_time(0), m_last_frame_time(0), m_delta_time(0),
m_frame_time(0.0166) {};

bool Window::create(Dimensions dims, const char* title) {
    return create(dims.width, dims.height, title);
}

bool Window::create(int width, int height, const char* title) {

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);

    m_dimensions.width = width;
    m_dimensions.height = height;

    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(m_window, reinterpret_cast<void*>(this));
    glfwMakeContextCurrent(m_window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void Window::setEngine(BaseEngine* engine) {
    m_engine = engine;
}

BaseEngine* Window::getEngine() {
    return m_engine;
}

void Window::useDefaultSizeCallback() {
    if(m_window){
        glfwSetFramebufferSizeCallback(m_window, 
        [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });
    }
}

void Window::useMouseMoveCallback() {
    glfwSetCursorPosCallback(m_window, mouseMoveCallback);
}

void Window::useMouseButtonCallback() {
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
}

void Window::useKeyCallback() {
    glfwSetKeyCallback(m_window, keyCallback);
}

void Window::useCharacterCallback() {
    glfwSetCharCallback(m_window, characterCallback);
}

int Window::getKey(int k) const{
    return glfwGetKey(m_window, k);
}

GLFWwindow* Window::getWindow() const {
    return m_window;
}
Dimensions Window::getDimensions(){
    glfwGetFramebufferSize(m_window, &m_dimensions.width, &m_dimensions.height);
    return m_dimensions;
}

void Window::loop(void (*func)()) {
    if(m_window){
        while(!glfwWindowShouldClose(m_window)){
            func();
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }
}

void Window::loop(std::function<void()> func) {
    if (m_window) {
        while (!glfwWindowShouldClose(m_window)) {
            m_current_frame_time = static_cast<float>(glfwGetTime());
            m_delta_time = m_current_frame_time - m_last_frame_time;
            m_last_frame_time = m_current_frame_time;
            
            func();
            glfwSwapBuffers(m_window);
            glfwPollEvents();

            float end_process_time = static_cast<float>(glfwGetTime()) - m_current_frame_time;
            float resting_time = m_frame_time - end_process_time;
            if (resting_time > 0) {
                int end_milliseconds = resting_time*1000;
                std::this_thread::sleep_for(std::chrono::milliseconds(end_milliseconds));
            }
            else {
                //less than target time
                std::cout << "Losing frames" << std::endl;
            }
        }
    }
}

void Window::setFPS(unsigned int fps) {
    m_frame_time = (0.5f/static_cast<float>(fps));
}

float Window::getDeltaTime() const {
    return m_delta_time;
}

float Window::getCurrentTime() const {
    return m_current_frame_time;
}

void Window::mouseMoveCallback(GLFWwindow* window, double winX, double winY) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    BaseEngine* engine = handler->getEngine();
    if (engine != nullptr) {
        engine->onMouseMove(static_cast<float>(winX), static_cast<float>(winY));
    }
    else {
        std::cout << "Engine not set" << std::endl;
    }
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    BaseEngine* engine = handler->getEngine();
    if (engine != nullptr) {
        engine->onMouseButton(button, action, mods);
    }
    else {
        std::cout << "Engine not set" << std::endl;
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    BaseEngine* engine = handler->getEngine();
    if (engine != nullptr) {
        engine->onKey(key, scancode, action, mods);
    }
    else {
        std::cout << "Engine not set for key callback" << std::endl;
    }
}

void Window::characterCallback(GLFWwindow* window, unsigned int codepoint) {
    Window* handler = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    BaseEngine* engine = handler->getEngine();
    if(engine != nullptr) {
        engine->onChar(codepoint);
    }
    else {
        std::cout << "Engine not set for char callback" << std::endl;
    }
}