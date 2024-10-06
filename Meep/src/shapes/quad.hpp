#pragma once

#include "../shaders/shader.hpp"
#include "general.hpp"

class Quad{
public:
	Quad();
	static void draw();

	static void setup();
	static void clean();
private:
	static unsigned int VBO;
	static unsigned int VAO;
	static unsigned int EBO;
	static bool s_is_ready;
};