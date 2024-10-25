#pragma once
#include <vector>
#include <map>

#include "../meep/meep.hpp"
#include "../utils/timer.hpp"
#include "../math/random.hpp"
#include "../shapes/shapes.hpp"

#include "../window/window.hpp"
#include "../window/interface_window.hpp"

class MeepEngine {
public:
	MeepEngine();
	MeepEngine(float engine_width, float engine_height);
	void setup();
	void clean();
	void spawnFood();
	void step(float dt);
	std::map<unsigned int, Meep*> getMeeps() const;
	std::map<unsigned int, Food> getFoods() const;

	float getWidth() const;
	float getHeight() const;

	std::optional<unsigned int> getHoveredMeepId() const;

	void onMouseMove(float x, float y);
	//std::pair<float, float> viewToEnginePoint(float x, float y) const;

private:
	std::map<unsigned int, Meep*> m_meeps;
	std::map<unsigned int, Food> m_foods;

	std::optional<unsigned int> m_hovered_meep_id;

	//unsigned int m_view_width;
	//unsigned int m_view_height;
	float m_width;
	float m_height;

	float m_mouse_x;
	float m_mouse_y;

	Timer m_food_spawn_timer;
};

class AppEngine : public BaseEngine {
public:
	AppEngine();

	static Dimensions s_window_dimensions;
	static glm::mat4 getFullProjection();

	std::vector<InterfaceWindow> getWindows() const;
	void addWindow(InterfaceWindow& win);

	const MeepEngine& getMeepEngine() const;
	const InterfaceWindow& getMeepWindow() const;
	const InterfaceWindow& getInformationWindow() const;

	void update(float dt);

	void onMouseMove(float x, float y);
	void onMouseLeftClick(float x, float y);

	Position getMouse() const;

private:
	void setupWindows();

	Position m_mouse;

	std::vector<InterfaceWindow> m_windows;

	MeepEngine m_meep_engine;
	InterfaceWindow m_meep_window;

	InterfaceWindow m_information_window;
};