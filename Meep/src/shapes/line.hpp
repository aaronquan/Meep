#pragma once

#include "glm/glm.hpp"
#include "../shaders/library.hpp"
#include "../utils/colour.hpp"

#include "../math/vector2d.hpp"
#include "../math/geometry.hpp"
#include "circle.hpp"
#include "quad.hpp"

struct LineSegment {
	float x1;
	float y1;
	float x2;
	float y2;
	LineSegment();
	LineSegment(float _x1, float _y1, float _x2, float _y2);
};

struct Point {
	float x;
	float y;
	std::string toString() const;
};

class LineDrawer {
public:
	static void draw(float x1, float y1, float x2, float y2, const glm::mat4& mvp);
	static void drawLineSegment(const LineSegment& ls, const glm::mat4& mvp);
	static void setColour(const Colour& c);
	//static void setColour(Colour c);
	static void setup();
	static void clean();
private:


	static unsigned int VBO;
	static unsigned int VAO;
	static unsigned int EBO;
	static bool s_is_ready;
	static std::optional<ShaderLibrary::MVPShader> m_line_shader;
};

class LineCircleDrawer {
public:
	static void draw(float x, float y, float radius, const glm::mat4& mvp, unsigned int n_points = 10);
	static void setColour(const Colour& c);
private:

};

enum class LineSide {
	Left, Right, OnPoint, Front, Behind
};

enum class LineCircleCollisionType {
	NoCollision, Touching, Intersecting
};

std::string lineCircleCollisionTypeToString(LineCircleCollisionType type);

struct LineCircleCollisionDetails {
	LineCircleCollisionType type;
	Point p1; 
	Point p2;
};

enum class LineRectCollisionType {
	NoCollision, TouchingCorner, Intersecting, OnHorizontal, OnVertical
};

std::string lineRectCollisionTypeToString(LineRectCollisionType type);

struct LineRectCollisionDetails {
	LineRectCollisionType type;
	Point p1; // used for corner points in onhori, onvert
	Point p2;
};

enum class GradientInterceptType {
	GradientIntercept, Vertical, NotLine
};

//y = mx + b or x
struct LineGradientInterceptForm {
	GradientInterceptType type;
	float m; // holds x in type
	float b;

};

class CollisionLine {
public:
	CollisionLine();
	CollisionLine(float x, float y, const Vector2D& dir, bool is_segment=false);

	static CollisionLine from2Points(float x1, float y1, float x2, float y2, bool is_segment=false);
	static CollisionLine fromLineSegment(const LineSegment& ls, bool is_segment=false);

	Point getStart() const;
	Point getEnd() const;
	const Vector2D& getDirection() const;

	float getX(float y) const;
	float getY(float x) const;


	LineSide sideOfPoint(float x, float y) const;
	float pointEquationValue(float x, float y) const;

	float closestDistanceToPoint(float px, float py) const;
	bool isPointOnLine(float px, float py, float error=0.001f) const;

	bool isVertical() const;
	bool isHorizontal() const;
	bool isLine() const;

	LineGradientInterceptForm getGradientInterceptForm() const;

	static const LineCircleCollisionDetails collisionLineToCircleDetails(const CollisionLine& line, const CollisionCircle& cc);
	static const LineRectCollisionDetails collisionLineToRectDetails(const CollisionLine& line, const CollisionRect& rect);
	
	//static std::vector<MapPosition> collision
private:
	float m_x1;
	float m_y1;
	float m_x2;
	float m_y2;
	bool m_segment;
	Vector2D m_direction;

	//ax + by + c = 0
	float m_a; //ax
	float m_b; //by
	float m_c; //c
};