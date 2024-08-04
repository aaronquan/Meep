#include "triangle.hpp"
#include <iostream>

bool Triangle::is_ready = false;
unsigned int Triangle::VBO;
unsigned int Triangle::VAO;

void Triangle::setup() {
    if(is_ready) return;
    float triangle_model[] = {
        -1.0f, -1.0f, 0.0f,  // left 
        1.0f, -1.0f, 0.0f,  // right
        -1.0f, 1.0f, 0.0f,  // top 
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_model), triangle_model, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    is_ready = true;

}

void Triangle::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    is_ready = false;
}

void Triangle::draw(Shader shader) {
    if (is_ready) {
        shader.setMatrix4("model", m_model);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}