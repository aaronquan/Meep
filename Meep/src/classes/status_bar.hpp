#pragma once
#include "../shaders/shader.hpp"
#include "../shapes/rectangle.hpp"
/*
class StatusBar {
public:
	StatusBar(float x, float y, float width, float height, float value): 
	m_background_rect(width+2, height+2),
	m_status_rect(width, height), 
	m_status(value > 1.0f || value < 0.0f ? 1.0f : value),
	m_width(width),
	m_x(x), m_y(y)
	
	{
		m_background_rect.setTranslate(x, y, 0);
		m_status_rect.setTranslate(x, y, 1);
	};
	void setStatusValue(float value) {
		if (value > 1.0f || value < 0.0f) return;
		float width = m_width * value;
		m_status = value;
		m_status_rect.setTranslate(m_x - (m_width/2 - width/2), m_y);
		m_status_rect.setWidth(width);
	}
	void draw(Shader shader) {
		if (is_ready) {
			float health_colour[] = { 0.1f, 0.9f, 0.1f };
			float health_back_colour[] = { 0.9f, 0.1f, 0.1f };

			shader.setFloat3("colour", health_back_colour[0], health_back_colour[1], health_back_colour[2]);
			m_background_rect.draw(shader);
			shader.setFloat3("colour", health_colour[0], health_colour[1], health_colour[2]);
			m_status_rect.draw(shader);
		}
	}
	void render() {
		if(is_ready){
			float health_colour[] = { 0.1f, 0.9f, 0.1f };
			float health_back_colour[] = { 0.9f, 0.1f, 0.1f };
			m_shader.use();




			m_shader.setFloat3("colour", health_back_colour[0], health_back_colour[1], health_back_colour[2]);
			m_background_rect.draw(m_shader);
			m_shader.setFloat3("colour", health_colour[0], health_colour[1], health_colour[2]);
			m_status_rect.draw(m_shader);
		}
	}



	static void setup() {
		Rectangle::setup();
		m_shader = Shader("./src/shaders/transformation.vs", "./src/shaders/solid_colour.fs");
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
		m_shader.use();
		m_shader.setMatrix4("model", model);
		m_shader.setMatrix4("view", view);
		m_shader.setMatrix4("projection", projection);
		is_ready = true;
	}
	static void setProjection(glm::mat4 projection){
		if (is_ready) {
			m_shader.use();
			m_shader.setMatrix4("projection", projection);
		}
	}
private:
	Rectangle m_background_rect;
	Rectangle m_status_rect;
	float m_width;
	float m_status;

	float m_x;
	float m_y;

	static Shader m_shader;
	static bool is_ready;
};
*/