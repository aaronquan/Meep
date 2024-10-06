#pragma once

#include "../shaders/shader.hpp"
#include "general.hpp"

class Triangle : public DrawShape2D {
public:
	Triangle() {};
    void draw();

    static void setup();
    static void clean();
private:
    static unsigned int VBO;
    static unsigned int VAO;
    static bool is_ready;
};