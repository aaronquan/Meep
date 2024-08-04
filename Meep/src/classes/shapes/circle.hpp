#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream> // debugging

#include "./../shader/shader.hpp"
#include "general.hpp"

class Circle : public DrawShape2D {
public:
	Circle(){};
	void draw(Shader shader);

	static void setup();
	static void clean();
private:

	static std::vector<float> getCircleModel(int count);
	static std::vector<unsigned int> getCircleIndices(int count);

	static unsigned int VBO;
	static unsigned int VAO;
	static unsigned int EBO;
	static unsigned int n_points;
	static bool is_ready;

	static unsigned int point_count;
};


class CollisionCircle {
public:
	CollisionCircle(float x, float y, float rad) : m_x(x), m_y(y), m_radius(rad){};

	bool collidesCircle(CollisionCircle circle);

private:
	float m_x;
	float m_y;
	float m_radius;
};