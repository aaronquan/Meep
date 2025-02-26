#pragma once
#include <iostream>
#include <glm/glm.hpp>

void printMat4(const glm::mat4& mat) {
	for (int i = 0; i < 4; i++) {
		std::cout << mat[i][0] << ' ' << mat[i][1] << ' ' << mat[i][2] << ' ' << mat[i][3] << '\n';
	}
}

void printVec3(const glm::vec3& vec) {
	std::cout << vec[0] << ',' << vec[1] << ',' << vec[2] << '\n';
}