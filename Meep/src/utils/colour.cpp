
#include "colour.hpp"

uint8_t Colour::s_max = 255;

Colour::Colour(): Colour(0, 0, 0){};

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a): 
m_r(r), m_g(g), m_b(b), m_a(a) {};

glm::vec3 Colour::toVec3() const {
	return glm::vec3(
		static_cast<float>(m_r)/s_max, 
		static_cast<float>(m_g)/s_max, 
		static_cast<float>(m_b) /s_max
	);
}

