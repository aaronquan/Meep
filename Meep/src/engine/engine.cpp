#include "engine.hpp"

Dimensions AppEngine::s_window_dimensions(800, 800);

AppEngine::AppEngine(): m_meep_engine(100, 100), m_meep_window(), m_information_window(){
	setupWindows();
	m_meep_engine.setup();
};

glm::mat4 AppEngine::getFullProjection() {
	return glm::ortho(
		0.0f,
		static_cast<float>(AppEngine::s_window_dimensions.width),
		static_cast<float>(AppEngine::s_window_dimensions.height),
		0.0f, 
		std::numeric_limits<float>::min(), 
		std::numeric_limits<float>::max()
	);
}

void AppEngine::setupWindows() {
	//InterfaceWindow win1(10, 25, s_window_dimensions.width-50, s_window_dimensions.height - 50);
	InterfaceWindow win1(10, 10, 100, 100);
	win1.setBackgroundColour(Colour(10, 140, 100));
	m_windows.push_back(win1);

	m_meep_window.setDimensions(
		AppEngine::s_window_dimensions.width-200, 
		AppEngine::s_window_dimensions.height-200
	);
	m_meep_window.setPosition(
		50, 50
	);
	m_meep_window.setBackgroundColour(Colour(50, 255, 50));

	m_information_window.setDimensions(200, 100);
	m_information_window.setPosition(200, AppEngine::s_window_dimensions.height - 150);
	m_information_window.setBackgroundColour(Colour(255, 50, 50));
}

std::vector<InterfaceWindow> AppEngine::getWindows() const{
	return m_windows;
}

const InterfaceWindow& AppEngine::getMeepWindow() const {
	return m_meep_window;
}

const InterfaceWindow& AppEngine::getInformationWindow() const {
	return m_information_window;
}

void AppEngine::addWindow(InterfaceWindow& win) {
	m_windows.push_back(win);
}

const MeepEngine& AppEngine::getMeepEngine() const {
	return m_meep_engine;
}

void AppEngine::update(float dt) {
	m_meep_engine.step(dt);
}

void AppEngine::onMouseMove(float x, float y) {
	Position mouse = Position(x, y);
	m_mouse = mouse;
	if (m_meep_window.isInside(mouse)) {
		Position relative = m_meep_window.getRelativePosition(mouse);
		//std::cout << relative.x << ' ' << relative.y << std::endl;

		Dimensions meep_window_dimensions = m_meep_window.getDimensions();

		float width = m_meep_engine.getWidth();
		float height = m_meep_engine.getHeight();

		float width_ratio = width/static_cast<float>(meep_window_dimensions.width);
		float height_ratio = width/static_cast<float>(meep_window_dimensions.height);

		std::cout << relative.x*width_ratio << ' ' << relative.y*height_ratio << std::endl;
	}

}

void AppEngine::onMouseLeftClick(float x, float y) {
	
}

MeepEngine::MeepEngine(): MeepEngine(0,0){};

MeepEngine::MeepEngine(float engine_width, float engine_height) : 
m_food_spawn_timer(1000),
m_width(engine_width), m_height(engine_height),
m_hovered_meep_id(std::nullopt),
m_mouse_x(0), m_mouse_y(0)
{};

void MeepEngine::setup() {
	Meep* m = new Meep(3.0f, 10.0f, MeepStage::Adult);
	m->setPosition(m_width/2, m_height/2);
	m_meeps[m->getId()] = m;
	//Meep m2;
	//m2.setPosition(m_width / 2 + 20, m_height / 2 + 20);
	//m_meeps[m2.getId()] = m2;
}
void MeepEngine::clean() {

}

void MeepEngine::spawnFood() {
	float x = Random::getFloat() * m_width;
	float y = Random::getFloat() * m_height;
	Food f(2);
	f.setPosition(x, y);
	m_foods[f.getId()] = f;
}

void MeepEngine::step(float dt) {
	unsigned int food_spawns = m_food_spawn_timer.add_time(dt * 1000);
	if (food_spawns > 0) {
		spawnFood();
	}
	std::vector<unsigned int> dead_m_meeps;
	m_hovered_meep_id = std::nullopt;
	for (auto& [meep_id, meep] : m_meeps) {
		MeepStateChangeData state_change_data = meep->updateState();
		if (state_change_data.clone_meep != nullptr) {
			Meep* clone = state_change_data.clone_meep;
			clone->setPosition(meep->getX() + meep->getSize(), meep->getY() + meep->getSize());
			m_meeps[clone->getId()] = clone;
			std::cout << "cloning" << std::endl;
		}


		MeepState meep_state = meep->getState();

		//meep->moveToClosestFood(m_foods);

		meep->step(dt);
		if (meep_state == MeepState::Dead) {
			dead_m_meeps.push_back(meep_id);
		}

		//check mouse hover
		if (meep->collidePoint(m_mouse_x, m_mouse_y)) {
			m_hovered_meep_id = meep_id;
		}

		//check collisions
		std::vector<unsigned int> collide_food_ids = meep->collideFood(m_foods, dt);
		for (unsigned int i : collide_food_ids) {
			m_foods.erase(i);
		}
	}
	for (unsigned int i : dead_m_meeps) {
		m_meeps.erase(i);
	}
}

std::map<unsigned int, Meep*> MeepEngine::getMeeps() const {
	return m_meeps;
}

std::map<unsigned int, Food> MeepEngine::getFoods() const {
	return m_foods;
}

std::optional<unsigned int> MeepEngine::getHoveredMeepId() const {
	return m_hovered_meep_id;
}

float MeepEngine::getWidth() const {
	return m_width;
}
float MeepEngine::getHeight() const {
	return m_height;
}

/*
std::pair<float, float> MeepEngine::viewToEnginePoint(float x, float y) const {
	float width_ratio = m_width / static_cast<float>(m_view_width);
	float height_ratio = m_height / static_cast<float>(m_view_height);
	return std::make_pair(x*width_ratio, y*height_ratio);
}
*/

void MeepEngine::onMouseMove(float x, float y) {
	//std::pair<float, float> engine_point = viewToEnginePoint(x, y);
	//float ex = engine_point.first;
	//float ey = engine_point.second;
	//m_mouse_x = ex; 
	//m_mouse_y = ey;

}