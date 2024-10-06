#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <functional>

struct Dimensions {
	int width;
	int height;
	Dimensions(int w, int h): width(w), height(h){};
};

struct Position {
	float x;
	float y;
	Position(): Position(0, 0){};
	Position(float _x, float _y): x(_x), y(_y){};
	Position(int _x, int _y): Position(static_cast<float>(_x), static_cast<float>(_y)){};
};

class BaseEngine {
public:
	virtual void onMouseMove(float x, float y) = 0;
	virtual void onMouseLeftClick(float x, float y) = 0;
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

	int getKey(int key) const;
	
	void loop(void (*func)());
	void loop(std::function<void()> func);
	
private:
	static void mouseMoveCallback(GLFWwindow* window, double winX, double winY);


	GLFWwindow* m_window;
	BaseEngine* m_engine;

	Dimensions m_dimensions;
};