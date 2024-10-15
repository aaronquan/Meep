#include "circle.hpp"

//Circle

bool QuadCircle::is_ready = false;
unsigned int QuadCircle::VBO;
unsigned int QuadCircle::VAO;
unsigned int QuadCircle::EBO;
unsigned int QuadCircle::n_points;

unsigned int QuadCircle::point_count = 12;

void QuadCircle::draw() {
    if (is_ready) {
        //shader.setMatrix4("model", m_model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, n_points, GL_UNSIGNED_INT, 0);
    }
}

std::vector<float> QuadCircle::getCircleModel(int count) {
    float angle = 0.0f;
    float angle_increment = 360.0f / count;
    int n_points = count*3;
    std::vector<float> points;
    for (int i = 0; i < count; i++) {
        float x = cos(glm::radians(angle));
        float y = sin(glm::radians(angle));
        points.push_back(x);
        points.push_back(y);
        points.push_back(0.0f);
        angle += angle_increment;
    }
    return points;
}

std::vector<unsigned int> QuadCircle::getCircleIndices(int count) {
    unsigned int n_triangles = (count - 2);
    n_points = n_triangles * 3;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < n_triangles; i++) {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
    return indices;
}

void QuadCircle::setup() {
    if(is_ready) return;
    std::vector<float> model = getCircleModel(point_count);
    std::vector<unsigned int> indices = getCircleIndices(point_count);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.size(), &model[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);

    is_ready = true;
}

void QuadCircle::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    is_ready = false;
}


//CollisionCircle

CollisionCircle::CollisionCircle() : CollisionCircle(0.0f, 0.0f, 1.0f){};

CollisionCircle::CollisionCircle(float x, float y, float rad) : m_x(x), m_y(y), m_radius(rad) {};

bool CollisionCircle::collidesPoint(float x, float y) const {
    float dx = x - m_x;
    float dy = y - m_y;
    return dx * dx + dy * dy < m_radius * m_radius;
}

bool CollisionCircle::collidesCircle(CollisionCircle circle) const {
    float rad_add = m_radius + circle.m_radius;
    float rad_sq = rad_add * rad_add;
    float dx = circle.m_x - m_x;
    float dy = circle.m_y - m_y;
    float dist_sq = dx * dx + dy * dy;
    return dist_sq < rad_sq;
}

void CollisionCircle::moveTo(float x, float y) {
    m_x = x; m_y = y;
}
void CollisionCircle::setRadius(float r) {
    m_radius = r;
}