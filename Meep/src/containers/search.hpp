#include <vector>

//assumes vec is sorted
//needs testing
size_t regionSearch(std::vector<float>& vec, float find) {
	if (vec.size() == 0) {
		return 0;
	}
	if (find > vec[vec.size() - 1]) {
		return vec.size() - 1;
	}

	size_t i = 0;
	size_t j = vec.size() - 1;
	while (i <= j) {
		size_t mid = (i + j) >> 1;
		//std::cout << mid << std::endl;
		if (find == vec[mid]) {
			return mid;
		}
		if (find > vec[mid]) {
			i = mid + 1;
		}
		else {
			j = mid - 1;
		}
	}
	if (find - vec[i] > vec[i + 1] - find) {
		return i + 1;
	}
	return i;
}