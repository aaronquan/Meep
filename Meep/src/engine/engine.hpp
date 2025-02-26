#pragma once
#include <vector>
#include <map>

#include "../meep/meep.hpp"
#include "../utils/timer.hpp"
#include "../math/random.hpp"
#include "../shapes/shapes.hpp"

#include "../window/window.hpp"
#include "../window/interface_window.hpp"

#include "test.hpp"
#include "../text/text_input.hpp"

#include "../math/matrix.hpp"

#include "./../ui_elements/button.hpp";

//include "general_engines.hpp"

struct Scale2D {
	float x;
	float y;
};

class MeepEngine: public Engine2D {
public:
	MeepEngine();
	MeepEngine(float engine_width, float engine_height);
	void setup();
	void clean();
	void spawnFood();
	void step(float dt);
	std::map<unsigned int, Meep*> getMeeps() const;
	std::map<unsigned int, Food*> getFoods() const;

	//std::vector<unsigned int> getActiveMeepIds() const;

	float getWidth() const;
	float getHeight() const;

	std::optional<unsigned int> getHoveredMeepId() const;

	void mouseEvent(const MapPosition& mouse);

	void onMouseMove(float x, float y);
	//std::pair<float, float> viewToEnginePoint(float x, float y) const;
	//void setMeepVelocity(Meep& meep);

private:
	std::map<unsigned int, Meep*> m_meeps;
	std::map<unsigned int, Food*> m_foods;

	std::optional<unsigned int> m_hovered_meep_id;

	//unsigned int m_view_width;
	//unsigned int m_view_height;
	float m_width;
	float m_height;

	float m_mouse_x;
	float m_mouse_y;

	float m_engine_time;

	Timer m_food_spawn_timer;

	void setMeepVelocity(Meep& meep);
};

class AppEngine : public BaseEngine {
public:
	AppEngine();

	static Dimensions s_window_dimensions;
	static glm::mat4 getFullProjection();
	static glm::mat4 getEngineView(const InterfaceWindow& window,
		float engineWidth, float engineHeight, float zoom=1.0f,
		float x=0.0f, float y=0.0f);

	static glm::mat4 getEngineViewFromBound(const InterfaceWindow& window,
		const Engine2DWorldViewBound& engine);

	std::vector<InterfaceWindow> getWindows() const;
	void addWindow(InterfaceWindow& win);

	const MeepEngine& getMeepEngine() const;
	CircleTestEngine& getCircleTestEngine();

	const InterfaceWindow& getMeepWindow() const;
	const InterfaceWindow& getInformationWindow() const;
	const InterfaceWindow& getCircleTestWindow() const;

	void setCircleTestWindowSize(int width, int height);
	void setCircleTestWindowPosition(int width, int height);

	void update(float dt);

	void onMouseMove(float x, float y);
	void onMouseButton(int button, int action, int mods);
	void onKey(int key, int scancode, int action, int mods);
	void onChar(unsigned int codepoint);

	WindowPosition* getMeepEngineMouse() const;
	std::optional<MapPosition> getCircleTestEngineMouse() const;

	WindowPosition getMouse() const;

	float getFPS() const;

	std::vector<TextInput>& getTextInputs();
	std::vector<NumberInput>& getNumberInputs();
	std::vector<Button>& getButtons();
	//NumberInput& getNumberInput(unsigned int i);

	static Scale2D getAppToEngineScale(const InterfaceWindow& iw, const Engine2D& engine);

private:
	void setupWindows();
	void setupTextInputs();
	void setupButtons();
	std::optional<MapPosition> getWindowEngineMouse(const InterfaceWindow& iw, Engine2D& engine) const;


	WindowPosition m_mouse;

	std::vector<InterfaceWindow> m_windows;

	MeepEngine m_meep_engine;

	CircleTestEngine m_circle_test_engine;
	InterfaceWindow m_circle_test_window;

	InterfaceWindow m_meep_window;

	InterfaceWindow m_information_window;
	std::vector<TextInput> m_text_inputs;
	std::vector<NumberInput> m_number_inputs;

	std::vector<Button> m_buttons;

	float m_fps;
};