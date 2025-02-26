#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream> // debugging

#include "../math/vector2d.hpp"
#include "../math/map.hpp"

#include "../shaders/shader.hpp"
#include "general.hpp"
//#include "line.hpp"

#include "../utils/colour.hpp"

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

enum class CircleCollisionType {
	NoCollision, Inside, Touching, Equals, Intersecting
};

class CollisionCircle;

struct CircleToCircleCollisionDetails {
	float distance;
	float distance_squared;
	Vector2D direction;
	CircleCollisionType type;
	const CollisionCircle* circle1;
	const CollisionCircle* circle2;
	CircleToCircleCollisionDetails(): 
		circle1(nullptr), circle2(nullptr),
		distance(0), distance_squared(0), 
		direction(Vector2D()), type(CircleCollisionType::NoCollision) 
	{};
};


class CollisionCircle {
public:
	CollisionCircle();
	CollisionCircle(float x, float y, float rad);
	bool collidesPoint(float x, float y) const;
	bool collidesCircle(const CollisionCircle& circle) const;
	const CircleToCircleCollisionDetails collidesCircleDetails(const CollisionCircle& circle) const;
	void moveTo(float x, float y);
	void setRadius(float r);

	float getX() const;
	float getY() const;
	float getRadius() const;

	static bool isCollision(const CollisionCircle& c1, const CollisionCircle& c2);
	static const CircleToCircleCollisionDetails collisionCircleDetails(const CollisionCircle& c1, const CollisionCircle& c2);
	static std::vector<MapPosition> collisionCirclePoints(const CircleToCircleCollisionDetails& details);

private:
	float m_x;
	float m_y;
	float m_radius;
};