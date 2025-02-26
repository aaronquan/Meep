#include "distances.hpp"
namespace Distances{

	Distance2D pointToPointDistance(float x1, float y1, float x2, float y2) {
		return Distance2D(abs(x2 - x1), abs(y2 - y1));
	}
}