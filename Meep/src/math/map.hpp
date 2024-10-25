#pragma once

struct MapPosition {
	float x;
	float y;
	MapPosition() : MapPosition(0, 0) {};
	MapPosition(float _x, float _y) : x(_x), y(_y) {};
};