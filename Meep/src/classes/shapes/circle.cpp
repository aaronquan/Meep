#include "circle.hpp"

//Circle

bool Circle::is_ready = false;
unsigned int Circle::VBO;
unsigned int Circle::VAO;
unsigned int Circle::EBO;
unsigned int Circle::n_points;

unsigned int Circle::point_count = 12;

void Circle::draw(Shader shader) {
    if (is_ready) {
        shader.setMatrix4("model", m_model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, n_points, GL_UNSIGNED_INT, 0);
    }
}

std::vector<float> Circle::getCircleModel(int count) {
    float angle = 0.0f;
    float angle_increment = 360.0f / count;
    size_t n_points = count*3;
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

std::vector<unsigned int> Circle::getCircleIndices(int count) {
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

void Circle::setup() {
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

void Circle::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    is_ready = false;
}


//CollisionCircle

bool CollisionCircle::collidesCircle(CollisionCircle circle) {
    float rad_add = m_radius + circle.m_radius;
    float rad_sq = rad_add * rad_add;
    float dx = circle.m_x - m_x;
    float dy = circle.m_y - m_y;
    float dist_sq = dx * dx + dy * dy;
    return dist_sq < rad_sq;
}