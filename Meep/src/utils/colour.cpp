
#include "colour.hpp"

int Colour::s_max_int = 255;
float Colour::s_max = 1.0f;

Colour::Colour(): Colour(0, 0, 0){};

Colour::Colour(float r, float g, float b, float a):
m_r(toValidFloat(r)), 
m_g(toValidFloat(g)), 
m_b(toValidFloat(b)), 
m_a(toValidFloat(a)) 
{};

Colour Colour::fromInt(int r, int g, int b, int a) {
	return Colour(intToColourFloat(r), intToColourFloat(g), intToColourFloat(b));
}

void Colour::add(float r, float g, float b, float a) {
	float aa = toValidFloat(a);
	float ar = toValidFloat(r) * aa;
	m_r = toValidFloat(m_r + ar);
	float ag = toValidFloat(g) * aa;
	m_g = toValidFloat(m_g + ag);
	float ab = toValidFloat(b) * aa;
	m_b = toValidFloat(m_b + ab);
}

const glm::vec3& Colour::toVec3() const {
	return glm::vec3(
		m_r, 
		m_g, 
		m_b
	);
}

const glm::vec4& Colour::toVec4() const {
	return glm::vec4(
		m_r,
		m_g,
		m_b,
		m_a
	);
}

void Colour::setAlpha(float a) {
	m_a = toValidFloat(a);
}

void Colour::setRed(float r) {
	m_r = toValidFloat(r);
}

void Colour::setGreen(float g) {
	m_g = toValidFloat(g);
}

void Colour::setBlue(float b) {
	m_b = toValidFloat(b);
}

float Colour::intToColourFloat(int v) {
	if (v > 255) return 1.0f;
	if (v < 0) return 0.0f;
	return static_cast<float>(v) / static_cast<float>(s_max_int);
}

float Colour::toValidFloat(float v) {
	if (v > 1.0f) return 1.0f;
	if (v < 0) return 0.0f;
	return v;
}

uint8_t Colour::floatToColourInt(float v) {
	return v > 1 ? (uint8_t)255 : static_cast<uint8_t>(v * Colour::s_max);
}

Colour Colour::white() {
	return Colour(1.0f, 1.0f, 1.0f);
}

Colour Colour::black() {
	return Colour(0.0f, 0.0f, 0.0f);
}

Colour Colour::red() {
	return Colour(1.0f, 0.0f, 0.0f);
}

Colour Colour::green() {
	return Colour(0.0f, 1.0f, 0.0f);
}

Colour Colour::blue() {
	return Colour(0.0f, 0.0f, 1.0f);
}

Colour Colour::clear() {
	return Colour(0.0f, 0.0f, 0.0f, 0.0f);
}