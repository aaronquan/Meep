#pragma once
#include <glm/glm.hpp>
#include "engine.hpp"

#include "shader/shader.hpp"
#include "shapes/shapes.hpp"

#include "meep/meep.hpp"
#include "meep/food.hpp"

#include "text.hpp"
#include "status_bar.hpp"

#include "shapes/shapes.hpp"

class Renderer2D {
public:
	Renderer2D() :  
		m_colour_shader("./src/shaders/transformation.vs", "./src/shaders/solid_colour.fs"),
		m_canvas_shader()
		//m_meep_shader("./src/shaders/transformation.vs", "./src/shaders/meep_fragment.fs"),
		//m_food_shader("./src/shaders/transformation.vs", "./src/shaders/food_fragment.fs") 
		{};

	void setup(Engine eng) {
		Triangle::setup();
		Circle::setup();
		Square::setup();
		StatusBar::setup();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::ortho(0.0f, eng.getWidth(), eng.getHeight(), 0.0f, -100.0f, 100.0f);
		std::cout << eng.getWidth() << ' ' << eng.getHeight() << std::endl;
		m_colour_shader.use();
		m_colour_shader.setMatrix4("model", model);
		m_colour_shader.setMatrix4("view", view);
		m_colour_shader.setMatrix4("projection", projection);

		StatusBar::setProjection(projection);

		/*
		m_meep_shader.use();
		m_meep_shader.setMatrix4("model", model);
		m_meep_shader.setMatrix4("view", view);
		m_meep_shader.setMatrix4("projection", projection);
		m_food_shader.use();
		m_food_shader.setMatrix4("model", model);
		m_food_shader.setMatrix4("view", view);
		m_food_shader.setMatrix4("projection", projection);
		*/
	}

	void clean() {
		Triangle::clean();
		Circle::clean();
		Square::clean();
	}

	void render(Engine eng){

		float meep_colour[] = { 0.4f, 0.3f, 0.9f };

		float food_colour[] = { 0.7f, 0.5f, 0.2f };

		float health_colour[] = { 0.1f, 0.9f, 0.1f };
		float health_back_colour[] = { 0.9f, 0.1f, 0.1f };

		m_colour_shader.use();
		m_colour_shader.setFloat3("colour", meep_colour[0], meep_colour[1], meep_colour[2]);
		for (auto const& [id, meep] : eng.getMeeps()) {
			float size = meep.getSize();
			Circle circ;
			circ.setTranslateScale(meep.getX(), meep.getY(), size, size);
			//circ.updateTransformation();
			circ.draw(m_colour_shader);
			

			float energy = meep.getEnergy();
			float max_energy = meep.getMaxEnergy();
			float energy_ratio = energy/max_energy;

			//StatusBar sb(15.0f, 5.0f, energy_ratio);
			//sb.render();

			/*
			m_colour_shader.setFloat3("colour", health_back_colour[0], health_back_colour[1], health_back_colour[2]);
			Rectangle rect2(max_energy, 3);
			rect2.setTranslate(15.0f, 5.0f);
			rect2.draw(m_colour_shader);

			m_colour_shader.setFloat3("colour", health_colour[0], health_colour[1], health_colour[2]);
			Rectangle rect(energy, 3);
			rect.setTranslate(15.0f-(max_energy - energy), 5.0f);
			//rect.setTranslate(15.0f, 5.0f);
			rect.draw(m_colour_shader);
			*/
		}
		m_colour_shader.setFloat3("colour", food_colour[0], food_colour[1], food_colour[2]);
		for (auto const& [id, food] : eng.getFoods()) {
			float size = food.getSize();
			Circle circ;
			circ.setTranslateScale(food.getX(), food.getY(), size, size);
			circ.draw(m_colour_shader);
		}
		//std::cout << eng.getFoods().size() << std::endl;
		//DrawText::renderText("Hello World", 0.0f, 0.0f);

	};

private:
	Shader m_colour_shader;
	Shader m_
	//Shader m_meep_shader;
	//Shader m_food_shader;
};