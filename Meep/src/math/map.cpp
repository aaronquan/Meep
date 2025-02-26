#include "map.hpp"

MapPosition::MapPosition() : MapPosition(0, 0) {}

MapPosition::MapPosition(float _x, float _y) : x(_x), y(_y) {}

bool MapPosition::isEqual(float _x, float _y) const {
	return x == _x && y == _y;
}

MapPosition operator+(const MapPosition& mp, const Vector2D& v) {
	return MapPosition(mp.x + v.getX(), mp.y + v.getY());
}