#include "matrix.hpp"

glm::vec3 getScaleFromMatrix(const glm::mat4& matrix) {
	float scale_x = glm::length(glm::vec3(matrix[0]));
	float scale_y = glm::length(glm::vec3(matrix[1]));
	float scale_z = glm::length(glm::vec3(matrix[2]));
	return glm::vec3(scale_x, scale_y, scale_z);
}

glm::mat2 rotationMat2FromAngle(float angle) {
	glm::mat2 mat;
	float s = sin(angle);
	float c = cos(angle);
	mat[0][0] = c; mat[0][1] = -s;
	mat[1][0] = s; mat[1][1] = c;
	return mat;
}