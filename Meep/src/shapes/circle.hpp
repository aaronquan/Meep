#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream> // debugging

#include "../shaders/shader.hpp"
#include "general.hpp"

class QuadCircle : public DrawShape2D {
public:
	QuadCircle(){};
	void draw();

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
	CollisionCircle();
	CollisionCircle(float x, float y, float rad);
	bool collidesPoint(float x, float y) const;
	bool collidesCircle(CollisionCircle circle) const;
	void moveTo(float x, float y);
	void setRadius(float r);

private:
	float m_x;
	float m_y;
	float m_radius;
};