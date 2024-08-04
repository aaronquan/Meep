#include "engine.hpp"

Engine::Engine(): Engine(100, 100){};

Engine::Engine(unsigned int width, unsigned int height) : 
m_food_spawn_timer(1000),
m_width(static_cast<float>(width)), m_height(static_cast<float>(height))
{};

void Engine::setup() {
	Meep m;
	m.setPosition(400.0f, 400.0f);
	m_meeps[m.getId()] = m;
}
void Engine::clean() {

}

void Engine::spawn_food() {
	float x = Random::getFloat() * m_width;
	float y = Random::getFloat() * m_height;
	Food f;
	f.setPosition(x, y);
	m_foods[f.getId()] = f;
}

void Engine::step(float dt) {
	unsigned int food_spawns = m_food_spawn_timer.add_time(dt * 1000);
	if (food_spawns > 0) {
		spawn_food();
	}
	std::vector<unsigned int> dead_m_meeps;
	for (auto& [meep_id, meep] : m_meeps) {
		meep.updateState();
		MeepState meep_state = meep.getState();

		meep.moveToClosestFood(m_foods);

		meep.step(dt);
		if (meep_state == Dead) {
			dead_m_meeps.push_back(meep_id);
		}

		//check collisions
		std::vector<unsigned int> collide_food_ids = meep.collideFood(m_foods);
		for (unsigned int i : collide_food_ids) {
			m_foods.erase(i);
		}
	}
	for (unsigned int i : dead_m_meeps) {
		m_meeps.erase(i);
	}
}

std::map<unsigned int, Meep> Engine::getMeeps() const {
	return m_meeps;
}

std::map<unsigned int, Food> Engine::getFoods() const {
	return m_foods;
}

float Engine::getWidth() const {
	return m_width;
}
float Engine::getHeight() const {
	return m_height;
}