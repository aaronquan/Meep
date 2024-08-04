#pragma once

#include "./../shader/shader.hpp"
#include "general.hpp"

class Square : public DrawShape2D{
public:
	Square();
	void draw(Shader shader);

	static void setup();
	static void clean();
private:
	static unsigned int VBO;
	static unsigned int VAO;
	static unsigned int EBO;
	static bool is_ready;
};