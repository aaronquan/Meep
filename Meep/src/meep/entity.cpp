#include "entity.hpp"

Entity2D::Entity2D() : Entity2D(0, 0) {};
Entity2D::Entity2D(float x, float y) : m_position(x, y) {};

float Entity2D::getX() const {
	return m_position.x;
}
float Entity2D::getY() const {
	return m_position.y;
}
void Entity2D::setX(float x) {
	m_position.x = x;
}
void Entity2D::setY(float y) {
	m_position.y = y;
}
MapPosition Entity2D::getPosition() {
	return m_position;
}
void Entity2D::setPosition(float x, float y) {
	m_position.x = x; m_position.y = y;
}

void Entity2D::move(float mx, float my) {
	m_position.x += mx; m_position.y += my;
}