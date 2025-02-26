#pragma once

#include <chrono>
#include <thread>
#include <optional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <functional>

struct Dimensions {
	int width;
	int height;
	Dimensions(int w, int h): width(w), height(h){};
};

struct WindowPosition {
	int x;
	int y;
	WindowPosition(): WindowPosition(0, 0){};
	WindowPosition(float _x, float _y) : WindowPosition(static_cast<int>(_x), static_cast<int>(_y)) {};
	WindowPosition(int _x, int _y): x(_x), y(_y){};
};

class BaseEngine {
public:
	virtual void onMouseMove(float x, float y) = 0;
	virtual void onMouseButton(int button, int action, int mods) = 0;
	virtual void onKey(int key, int scancode, int action, int mods) = 0;
	virtual void onChar(unsigned int codepoint) = 0; //https://www.glfw.org/docs/latest/input_guide.html
};

class Window {
public:
	Window();

	void setEngine(BaseEngine* engine);
	BaseEngine* getEngine();

	GLFWwindow* getWindow() const;
	Dimensions getDimensions();

	bool create(int width, int height, const char* title);
	bool create(Dimensions dims, const char* title);
	void useDefaultSizeCallback();

	void useMouseMoveCallback();
	void useMouseButtonCallback();
	void useKeyCallback();
	void useCharacterCallback();

	int getKey(int key) const;

	
	void loop(void (*func)());
	void loop(std::function<void()> func);

	void setFPS(unsigned int fps);

	float getDeltaTime() const;
	float getCurrentTime() const;
	
private:
	static void mouseMoveCallback(GLFWwindow* window, double winX, double winY);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void characterCallback(GLFWwindow* window, unsigned int codepoint);

	GLFWwindow* m_window;
	BaseEngine* m_engine;

	Dimensions m_dimensions;
	float m_current_frame_time;
	float m_last_frame_time;
	float m_delta_time;
	float m_frame_time;
};