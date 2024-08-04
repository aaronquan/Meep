#pragma once

#include "square.hpp"

class Rectangle: public Square {
public:
	Rectangle(): Rectangle(1, 1){};
	Rectangle(float width, float height) : 
	m_width(width), m_height(height),
	m_x(0), m_y(0)
	{
		transformations();
	};
	void setTranslate(float x, float y) {
		m_x = x - m_width/2; m_y = y - m_height/2;
		transformations();
	}
	void setWidth(float width) {
		m_width = width;
		transformations();
	}
	void setHeight(float height) {
		m_height = height;
		transformations();
	}
private:
	void transformations() {
		m_model = glm::mat4(1.0f);
		m_model = glm::translate(m_model, glm::vec3(m_x, m_y, 0.0f));
		m_model = glm::scale(m_model, glm::vec3(m_width, m_height, 1.0f));
	}
	float m_x;
	float m_y;
	float m_width;
	float m_height;
};