#pragma once
#include "../math/map.hpp"

class Entity2D {
public:
	Entity2D();
	Entity2D(float x, float y);
	float getX() const;
	float getY() const;
	void setX(float x);
	void setY(float y);
	MapPosition getPosition();
	void setPosition(float x, float y);

protected:
	void move(float mx, float my);
	MapPosition m_position;
};
