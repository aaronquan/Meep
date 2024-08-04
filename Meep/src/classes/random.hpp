#pragma once
#include <cstdlib>

namespace Random {
	inline float getFloat() {
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
}