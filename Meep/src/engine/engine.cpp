#include "engine.hpp"

Dimensions AppEngine::s_window_dimensions(200, 250);

AppEngine::AppEngine(){};

void AppEngine::setupWindows() {
	InterfaceWindow win1(100, 100, 50, 50);
	m_windows.push_back(win1);
}

std::vector<InterfaceWindow> AppEngine::getWindows() const{
	return m_windows;
}

void AppEngine::onMouseMove(float x, float y) {

}

void AppEngine::onMouseLeftClick(float x, float y) {
	
}

Engine::Engine(): Engine(0,0, 0,0){};

Engine::Engine(unsigned int screen_width, unsigned int screen_height, 
float engine_width, float engine_height) : 
m_food_spawn_timer(1000),
m_view_width(screen_width), m_view_height(screen_height),
m_width(engine_width), m_height(engine_height),
m_hovered_meep_id(std::nullopt),
m_mouse_x(0), m_mouse_y(0)
{};

void Engine::setup() {
	Meep m;
	m.setPosition(m_width/2, m_height/2);
	m_meeps[m.getId()] = m;
	Meep m2;
	m2.setPosition(m_width / 2 + 20, m_height / 2 + 20);
	m_meeps[m2.getId()] = m2;
}
void Engine::clean() {

}

void Engine::spawnFood() {
	float x = Random::getFloat() * m_width;
	float y = Random::getFloat() * m_height;
	Food f(2);
	f.setPosition(x, y);
	m_foods[f.getId()] = f;
}

void Engine::step(float dt) {
	unsigned int food_spawns = m_food_spawn_timer.add_time(dt * 1000);
	if (food_spawns > 0) {
		spawnFood();
	}
	std::vector<unsigned int> dead_m_meeps;
	m_hovered_meep_id = std::nullopt;
	for (auto& [meep_id, meep] : m_meeps) {
		meep.updateState();
		MeepState meep_state = meep.getState();

		meep.moveToClosestFood(m_foods);

		meep.step(dt);
		if (meep_state == Dead) {
			dead_m_meeps.push_back(meep_id);
		}

		//check mouse hover
		if (meep.collidePoint(m_mouse_x, m_mouse_y)) {
			m_hovered_meep_id = meep_id;
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

std::optional<unsigned int> Engine::getHoveredMeepId() const {
	return m_hovered_meep_id;
}

float Engine::getWidth() const {
	return m_width;
}
float Engine::getHeight() const {
	return m_height;
}

std::pair<float, float> Engine::viewToEnginePoint(float x, float y) const {
	float width_ratio = m_width / static_cast<float>(m_view_width);
	float height_ratio = m_height / static_cast<float>(m_view_height);
	return std::make_pair(x*width_ratio, y*height_ratio);
}

void Engine::onMouseMove(float x, float y) {
	std::pair<float, float> engine_point = viewToEnginePoint(x, y);
	float ex = engine_point.first;
	float ey = engine_point.second;
	m_mouse_x = ex; 
	m_mouse_y = ey;

}