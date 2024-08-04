
#include "square.hpp"

bool Square::is_ready = false;
unsigned int Square::VBO;
unsigned int Square::VAO;
unsigned int Square::EBO;

Square::Square() {
    
}

void Square::draw(Shader shader){
    if (is_ready) {
        shader.setMatrix4("model", m_model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};

void Square::setup() {
    if (is_ready) return;
    float square_model[] = {
        -0.5f, -0.5f, 0.0f,  // bottom - left 
        -0.5f, 0.5f, 0.0f,  // top - left
        0.5f, 0.5f, 0.0f,  // top - right
        0.5f, -0.5f, 0.0f,  // bottom - right
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_model), square_model, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    is_ready = true;
}

void Square::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    is_ready = false;
}