#pragma once

class Entity2D {
public:
	Entity2D(): m_x(0), m_y(0){};
	Entity2D(float x, float y) : m_x(x), m_y(y){};
	float getX() const {
		return m_x;
	}
	float getY() const {
		return m_y;
	}
	void setPosition(float x, float y) {
		m_x = x; m_y = y;
	}
protected:
	float m_x;
	float m_y;
};
