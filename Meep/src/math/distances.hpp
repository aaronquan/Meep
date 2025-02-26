#pragma once
#include <cmath>

namespace Distances {
	struct Distance2D {
		float x;
		float y;

	};


	inline float pointToPointDistanceSquaredFloat(float x1, float y1, float x2, float y2) {
		float dx = x2 - x1;
		float dy = y2 - y1;
		return dx * dx + dy * dy;
	}

	inline float pointToPointDistanceFloat(float x1, float y1, float x2, float y2) {
		return sqrt(pointToPointDistanceSquaredFloat(x1, y1, x2, y2));
	}

	Distance2D pointToPointDistance(float x1, float y1, float x2, float y2);
}