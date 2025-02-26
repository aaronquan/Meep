#pragma once
#include <iostream>
#include <optional>

class Vector2D {
public:
	Vector2D();
	Vector2D(float x, float y);

	Vector2D operator+=(const Vector2D& v);
	Vector2D operator*=(const Vector2D& v);

	friend Vector2D operator+(Vector2D v1, const Vector2D& v2);
	friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
	friend Vector2D operator*(const Vector2D& v1, float scale);
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

	float getX() const;
	float getY() const;
	Vector2D opposite() const;
	Vector2D perpendicular() const;
	Vector2D normal() const;
	float getLength() const;
	float getLengthSquared() const;
	bool isZero() const;

private:
	float m_x;
	float m_y;
	std::optional<float> m_length;
};

//Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
//Vector2D operator*(const Vector2D& v1, float scale);

//std::ostream& operator<<(std::ostream& os, const Vector2D& vec);