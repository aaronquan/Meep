#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class DrawShape2D {
public:
    DrawShape2D(): m_model(glm::mat4(1.0f)), 
    m_translate_x(0.0f), m_translate_y(0.0f),
    m_scale_x(1.0f), m_scale_y(1.0f)
    {};
    void setTranslate(float x, float y) {
        m_translate_x = x;
        m_translate_y = y;
        updateTransformation();
    }
    void setScale(float x, float y) {
        m_scale_x = x;
        m_scale_y = y;
        updateTransformation();
    }
    void setTranslateScale(
        float translate_x, float translate_y,
        float scale_x, float scale_y
    ) {
        m_translate_x = translate_x;
        m_translate_y = translate_y;
        m_scale_x = scale_x;
        m_scale_y = scale_y;
        updateTransformation();
    }

protected:
    glm::mat4 m_model;
    
    float m_translate_x;
    float m_translate_y;
    float m_scale_x;
    float m_scale_y;

private:
    void updateTransformation() {
        m_model = glm::mat4(1.0f);
        m_model = glm::translate(m_model, glm::vec3(m_translate_x, m_translate_y, 0.0f));
        m_model = glm::scale(m_model, glm::vec3(m_scale_x, m_scale_y, 0.0f));
    }
};

class CollisionShape2D {
    
};