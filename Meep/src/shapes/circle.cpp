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

bool CollisionCircle::collidesCircle(const CollisionCircle& circle) const {
    float rad_add = m_radius + circle.m_radius;
    float rad_sq = rad_add * rad_add;
    float dx = circle.m_x - m_x;
    float dy = circle.m_y - m_y;
    float dist_sq = dx * dx + dy * dy;
    return dist_sq < rad_sq;
}

const CircleToCircleCollisionDetails CollisionCircle::collidesCircleDetails(const CollisionCircle& circle) const {
    return collisionCircleDetails(*this, circle);
}



void CollisionCircle::moveTo(float x, float y) {
    m_x = x; m_y = y;
}
void CollisionCircle::setRadius(float r) {
    m_radius = r;
}

float CollisionCircle::getX() const {
    return m_x;
}

float CollisionCircle::getY() const {
    return m_y;
}

float CollisionCircle::getRadius() const {
    return m_radius;
}

bool CollisionCircle::isCollision(const CollisionCircle& c1, const CollisionCircle& c2) {
    float rad_add = c1.m_radius + c2.m_radius;
    float rad_sq = rad_add * rad_add;
    float dx = c2.m_x - c1.m_x;
    float dy = c2.m_y - c1.m_y;
    float dist_sq = dx * dx + dy * dy;
    return dist_sq < rad_add;
}

const CircleToCircleCollisionDetails CollisionCircle::collisionCircleDetails(const CollisionCircle& c1, const CollisionCircle& c2) {
    CircleToCircleCollisionDetails details;
    float radius_add = c2.m_radius + c1.m_radius;
    float radius_squared = radius_add * radius_add;
    float dx = c2.m_x - c1.m_x;
    float dy = c2.m_y - c1.m_y;
    float distance_squared = dx * dx + dy * dy;
    details.circle1 = &c1;
    details.circle2 = &c2;

    if (dx == 0 && dy == 0 && c2.m_radius == c1.m_radius) {
        details.type = CircleCollisionType::Equals;
    }
    else if (distance_squared > radius_squared) {
        details.type = CircleCollisionType::NoCollision;
    }
    else if (distance_squared == radius_squared) {
        details.type = CircleCollisionType::Touching;
        details.distance = radius_add;
        details.distance_squared = radius_squared;
        details.direction = radius_add == 0 ? Vector2D() : Vector2D(dx / radius_add, dy / radius_add);
    }
    else {
        float radius_difference = abs(c2.m_radius - c1.m_radius);
        float distance = sqrt(distance_squared);
        if (distance_squared < radius_difference * radius_difference) {
            details.type = CircleCollisionType::Inside;
        }
        else {
            details.type = CircleCollisionType::Intersecting;
        }
        details.distance = distance;
        details.distance_squared = distance_squared;
        details.direction = distance == 0 ? Vector2D() : Vector2D(dx / distance, dy / distance);
    }
    return details;
}

std::vector<MapPosition> CollisionCircle::collisionCirclePoints(const CircleToCircleCollisionDetails& details) {
    std::vector<MapPosition> collision_points;
    switch (details.type) {
    case CircleCollisionType::Touching:
        //float distance = details.distance;
        break;
    case CircleCollisionType::Intersecting:
        float r1 = details.circle1->m_radius;
        float r2 = details.circle2->m_radius;
        float x1 = details.circle1->m_x; float y1 = details.circle1->m_y;
        float x2 = details.circle2->m_x; float y2 = details.circle2->m_y;

        float radius_diff = abs(r1 - r2);

        float dx = x2 - x1; float dy = y2 - y1;
        float dist = details.distance;
        float dist_squared = details.distance_squared;

        float r1_squared = r1 * r1;
        float r2_squared = r2 * r2;
        float r1r2_diff = r1_squared - r2_squared;

        float a = r1r2_diff / (2 * dist_squared);
        float b1 = (2 * (r1_squared + r2_squared)) / dist_squared;
        float b2 = (r1r2_diff * r1r2_diff) / (dist_squared * dist_squared);
        float b = sqrt(b1 - b2 - 1);

        float xa = ((x1 + x2) / 2) + a * dx;
        float xb = (b * (y2 - y1)) / 2;
        float cx1 = xa + xb;
        float cx2 = xa - xb;

        float ya = ((y1 + y2) / 2) + a * dy;
        float yb = (b * (x1 - x2)) / 2;
        float cy1 = ya + yb;
        float cy2 = ya - yb;
        MapPosition collision1;
        collision1.x = cx1;
        collision1.y = cy1;
        collision_points.push_back(collision1);
        MapPosition collision2;
        collision2.x = cx2;
        collision2.y = cy2;
        collision_points.push_back(collision2);
        break;
    }
    return collision_points;
}