#pragma once
#include "vector2d.hpp"
struct MapPosition {
	float x;
	float y;
	MapPosition();
	MapPosition(float _x, float _y);
	bool isEqual(float _x, float _y) const;
};

MapPosition operator+(const MapPosition& mp, const Vector2D& v);
