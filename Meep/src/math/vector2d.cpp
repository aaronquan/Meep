#include "vector2d.hpp"

Vector2D::Vector2D(): Vector2D(0, 0) {};

Vector2D::Vector2D(float x, float y) : m_x(x), m_y(y), m_length(std::nullopt) {};

Vector2D Vector2D::operator+=(const Vector2D& v) {
	m_x += v.m_x;
	m_y += v.m_y;
	return *this;
}

Vector2D Vector2D::operator*=(const Vector2D& v) {
	m_x *= v.m_x;
	m_y *= v.m_y;
	return *this;
}

float Vector2D::getX() const {
	return m_x;
}

float Vector2D::getY() const {
	return m_y;
}

Vector2D Vector2D::opposite() const {
	return Vector2D(-m_x, -m_y);
}

Vector2D Vector2D::perpendicular() const {
	return Vector2D(-m_y, m_x);
}

Vector2D Vector2D::normal() const {
	float length = getLength();
	if (length == 0) return Vector2D();
	return Vector2D(m_x/length, m_y/length);
}

float Vector2D::getLength() const {
	if (m_length.has_value()) {
		return m_length.value();
	}
	return sqrt(getLengthSquared());
}

float Vector2D::getLengthSquared() const {
	return m_x * m_x + m_y * m_y;
}

bool Vector2D::isZero() const {
	return m_x == 0 && m_y == 0;
}

Vector2D operator+(Vector2D v1, const Vector2D& v2) {
	v1 += v2;
	return v1;
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
	return Vector2D(v1.m_x + v2.m_x, v1.m_y + v2.m_y);
}

Vector2D operator*(const Vector2D& v, float scale) {
	return Vector2D(v.getX() * scale, v.getY() * scale);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
	os << "x: " << vec.getX() << ", y: " << vec.getY();
	return os;
}