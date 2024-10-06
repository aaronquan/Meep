#pragma once
//#include <cstdlib>
#include <cmath>

namespace Random {

inline float getFloat() {
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

inline float getFloatBetween(float max, float min = 0) {
	float r = getFloat();
	float diff = max - min;
	return r * diff + min;
}
}