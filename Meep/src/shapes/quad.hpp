#pragma once

#include "../shaders/shader.hpp"
#include "../engine/general_engines.hpp"

#include "../window/window.hpp"

class Quad{
public:
	Quad();
	static void draw();

	static void setup();
	static void clean();
private:
	static unsigned int VBO;
	static unsigned int VAO;
	static unsigned int EBO;
	static bool s_is_ready;
};

class CollisionRect {
public:
	CollisionRect();
	CollisionRect(float x, float y, float width, float height);
	static CollisionRect fromRectBound(const RectBound& rect_bound);
	bool isCollisionPoint(float x, float y) const;
	bool isInsideX(float x) const;
	bool isInsideY(float y) const;
	float getLeft() const;
	float getTop() const;
	float getRight() const;
	float getBottom() const;
private:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;

};