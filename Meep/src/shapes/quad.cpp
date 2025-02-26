
#include "quad.hpp"

bool Quad::s_is_ready = false;
unsigned int Quad::VBO;
unsigned int Quad::VAO;
unsigned int Quad::EBO;

Quad::Quad() {}

void Quad::draw(){
    if (!s_is_ready) {
        setup();
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};

void Quad::setup() {
    if (s_is_ready) return;
    float quad_data[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom - left 
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // top - left
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,  // top - right
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f // bottom - right
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(1);

    s_is_ready = true;
    //std::cout << "setup quad" << std::endl;
}

void Quad::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    s_is_ready = false;
}

CollisionRect::CollisionRect(): CollisionRect(0,0,1,1) {};

CollisionRect::CollisionRect(float x, float y, float width, float height) : 
    m_left(x), m_top(y), 
    m_right(x+width), m_bottom(y+height){};

CollisionRect CollisionRect::fromRectBound(const RectBound& rect_bound) {
    return CollisionRect(rect_bound.left, rect_bound.top, rect_bound.getWidth(), rect_bound.getHeight());
}

bool CollisionRect::isCollisionPoint(float x, float y) const {
    return isInsideX(x) && isInsideY(y);
}

bool CollisionRect::isInsideX(float x) const {
    return x >= m_left && x <= m_right;
}

bool CollisionRect::isInsideY(float y) const {
    return y >= m_top && y <= m_bottom;
}

float CollisionRect::getLeft() const {
    return m_left;
}

float CollisionRect::getTop() const {
    return m_top;
}

float CollisionRect::getRight() const {
    return m_right;
}

float CollisionRect::getBottom() const {
    return m_bottom;
}