#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Colour {
public:
	Colour();
	//Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	Colour(float r, float g, float b, float a = 1.0f);
	static Colour fromInt(int r, int g, int b, int a = 255);

	const glm::vec3& toVec3() const;
	const glm::vec4& toVec4() const;

	void setAlpha(float a);
	void setRed(float r);
	void setGreen(float g);
	void setBlue(float b);

	void add(float r, float g, float b, float a = 1.0f);

	static Colour white();
	static Colour black();
	static Colour red();
	static Colour blue();
	static Colour green();
	static Colour clear();

private:
	//old implementation
	//uint8_t m_r;
	//uint8_t m_g;
	//uint8_t m_b;
	//uint8_t m_a;

	float m_r;
	float m_g;
	float m_b;
	float m_a;

	static float intToColourFloat(int v);
	static uint8_t floatToColourInt(float v);
	static float toValidFloat(float v);
	
	static int s_max_int;
	static float s_max;
};