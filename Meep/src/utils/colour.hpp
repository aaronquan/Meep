#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Colour {
public:
	Colour();
	Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	glm::vec3 toVec3() const;

private:
	uint8_t m_r;
	uint8_t m_g;
	uint8_t m_b;
	uint8_t m_a;

	static uint8_t s_max;
};