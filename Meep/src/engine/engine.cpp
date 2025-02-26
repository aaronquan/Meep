#include "engine.hpp"

Dimensions AppEngine::s_window_dimensions(800, 800);

AppEngine::AppEngine() : m_meep_engine(100, 100), m_meep_window(), m_information_window(),
m_fps(0), m_text_inputs()
{
	setupWindows();
	setupTextInputs();
	setupButtons();
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

glm::mat4 AppEngine::getEngineView(const InterfaceWindow& window, 
	float engine_width, 
	float engine_height, 
	float zoom, float x, float y) {
	WindowPosition pos = window.getPosition();
	Dimensions dims = window.getDimensions();
	//float zoom = 1.0f;
	float scaleX = zoom * (static_cast<float>(dims.width) / AppEngine::s_window_dimensions.width);
	float scaleY = zoom * (static_cast<float>(dims.height) / AppEngine::s_window_dimensions.height);
	glm::mat4 view(1.0f);
	
	view = glm::translate(view, glm::vec3(
		pos.x * (engine_width / AppEngine::s_window_dimensions.width) -
		x * scaleX,
		pos.y * (engine_height / AppEngine::s_window_dimensions.height) -
		y * scaleY,
		0));
	
	//view = glm::translate(view, glm::vec3(pos.x, pos.y, 0));

	view = glm::scale(view, glm::vec3(
		scaleX,
		scaleY,
		1)
	);
	return view;
}

glm::mat4 AppEngine::getEngineViewFromBound(const InterfaceWindow& window,
	const Engine2DWorldViewBound& engine) {
	WindowPosition pos = window.getPosition();
	Dimensions dims = window.getDimensions();
	const RectBound& view_bound = engine.getViewBound();
	const RectBound& world_bound = engine.getWorldBound();
	float view_width = view_bound.getWidth();
	float view_height = view_bound.getHeight();
	float world_width = world_bound.getWidth();
	float world_height = world_bound.getHeight();

	float translate_x = view_bound.left;
	float translate_y = view_bound.top;

	float engine_scale_x = world_width / view_width;
	float engine_scale_y = world_height / view_height;

	float scale_x = (static_cast<float>(dims.width) / AppEngine::s_window_dimensions.width);
	float scale_y = (static_cast<float>(dims.height) / AppEngine::s_window_dimensions.height);

	std::cout << scale_x << ' ' << scale_y << '\n';

	//float zoom = 1.0f;
	//float scaleX = (static_cast<float>(dims.width) / AppEngine::s_window_dimensions.width);
	//float scaleY = (static_cast<float>(dims.height) / AppEngine::s_window_dimensions.height);
	glm::mat4 view(1.0f);

	view = glm::translate(view, glm::vec3(
		pos.x * (view_width / AppEngine::s_window_dimensions.width) + (translate_x * (1-scale_x)),
		pos.y * (view_height / AppEngine::s_window_dimensions.height) + (translate_y * (1 - scale_y)),
		0
	));

	//view = glm::translate(view, glm::vec3(pos.x, pos.y, 0));

	view = glm::scale(view, glm::vec3(
		scale_x,
		scale_y,
		1
	));
	return view;
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

const InterfaceWindow& AppEngine::getCircleTestWindow() const {
	return m_circle_test_window;
}

void AppEngine::setCircleTestWindowSize(int width, int height) {
	m_circle_test_window.setDimensions(width, height);
}

void AppEngine::setCircleTestWindowPosition(int width, int height) {
	m_circle_test_window.setPosition(width, height);
}

void AppEngine::addWindow(InterfaceWindow& win) {
	m_windows.push_back(win);
}

const MeepEngine& AppEngine::getMeepEngine() const {
	return m_meep_engine;
}

CircleTestEngine& AppEngine::getCircleTestEngine(){
	return m_circle_test_engine;
}

void AppEngine::update(float dt) {
	m_meep_engine.step(dt);
	m_circle_test_engine.step(dt);

	TextInput::cursorTick(dt);

	//Mouse Event
	std::optional<MapPosition> circle_test_mouse_position = getCircleTestEngineMouse();
	if (circle_test_mouse_position != std::nullopt) {
		m_circle_test_engine.mouseMoveEvent(circle_test_mouse_position.value());
	}

	std::optional<MapPosition> mmp = getWindowEngineMouse(m_meep_window, m_meep_engine);
	if (mmp != std::nullopt) {
		m_meep_engine.mouseEvent(mmp.value());
	}

	if(dt > 0) m_fps = 1 / dt;
}

void AppEngine::onMouseMove(float x, float y) {
	//Position mouse = Position(x, y);
	m_mouse = WindowPosition(x, y);

	for (Button& button : m_buttons) {
		button.mouseMoveEvent(x, y);
	}
	//if (circle_test_mouse_position != std::nullopt) {

	//}
	/*
	if (m_meep_window.isInside(mouse)) {
		Position relative = m_meep_window.getRelativePosition(mouse);
		//std::cout << relative.x << ' ' << relative.y << std::endl;

		Dimensions meep_window_dimensions = m_meep_window.getDimensions();

		float width = m_meep_engine.getWidth();
		float height = m_meep_engine.getHeight();

		float width_ratio = width/static_cast<float>(meep_window_dimensions.width);
		float height_ratio = width/static_cast<float>(meep_window_dimensions.height);

		//std::cout << relative.x*width_ratio << ' ' << relative.y*height_ratio << std::endl;
	}*/

}


void AppEngine::onMouseButton(int button, int action, int mods) {
	std::cout << "Mouse Clicked at: " << m_mouse.x << ", " << m_mouse.y << std::endl;
	bool hasTextCollision = false;
	for (NumberInput& ni : m_number_inputs) {
		std::optional<Vector2D> collision = ni.collisionPoint(m_mouse.x, m_mouse.y);
		if (collision.has_value()) {
			NumberInput::setHighlightedInput(&ni);
			hasTextCollision = true;
			std::cout << "Number Input Collision" << std::endl;
			break;
		}
	}
	if (!hasTextCollision) {
		for (TextInput& ti : m_text_inputs) {
			std::optional<Vector2D> collision = ti.collisionPoint(m_mouse.x, m_mouse.y);
			if (collision.has_value()) {
				NumberInput::setHighlightedInput(&ti);
				hasTextCollision = true;
				std::cout << "Text Input Collision" << std::endl;
				break;
			}
		}
	}
	if (!hasTextCollision) {
		hasTextCollision = m_circle_test_engine.mouseClickTextCollision(m_mouse);
	}
	if (!hasTextCollision){
		TextInput::noHighlightedInput();
	}
	if (action == GLFW_PRESS) {
		m_circle_test_engine.mousePressEvent();

		for (Button& button : m_buttons) {
			button.mouseDownEvent();
		}
	}
	else if (action == GLFW_RELEASE) {
		m_circle_test_engine.mouseReleaseEvent();
	}
}

void AppEngine::onKey(int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		const char* key_name = glfwGetKeyName(key, scancode);
		if (key_name != nullptr) {
			if (!NumberInput::addCharacterToHighlighted(*key_name)) {
				m_circle_test_engine.keyboardPressEvent(key_name);
			}
		}
		else {
			if (key == GLFW_KEY_SPACE) {
				NumberInput::addCharacterToHighlighted(' ');
			}
			else if (key == GLFW_KEY_BACKSPACE) {
				TextInput::addBackspaceToHighlighted();
			}
			else if (key == GLFW_KEY_LEFT) {
				TextInput::moveHighlightedLeft();
			}
			else if (key == GLFW_KEY_RIGHT) {
				TextInput::moveHighlightedRight();
			}
			else {
				std::cout << "Not identified" << std::endl;
			}
		}
	}
}

void AppEngine::onChar(unsigned int codepoint) {

}



WindowPosition* AppEngine::getMeepEngineMouse() const {
	if (!m_meep_window.isInside(m_mouse)) {
		return nullptr;
	}
	WindowPosition relative = m_meep_window.getRelativePosition(m_mouse);

	Dimensions meep_window_dimensions = m_meep_window.getDimensions();

	float width = m_meep_engine.getWidth();
	float height = m_meep_engine.getHeight();

	float width_ratio = width / static_cast<float>(meep_window_dimensions.width);
	float height_ratio = width / static_cast<float>(meep_window_dimensions.height);

	return new WindowPosition(relative.x * width_ratio, relative.y * height_ratio);
}

std::optional<MapPosition> AppEngine::getCircleTestEngineMouse() const {
	if (!m_circle_test_window.isInside(m_mouse)) {
		return std::nullopt;
	}
	WindowPosition relative = m_circle_test_window.getRelativePosition(m_mouse);

	Dimensions meep_window_dimensions = m_circle_test_window.getDimensions();
	const RectBound& view_bound = m_circle_test_engine.getViewBound();
	float width = view_bound.getWidth();
	float height = view_bound.getHeight();
	//glm::vec2 scale = m_circle_test_engine.getScale();

	float width_ratio = (width / static_cast<float>(meep_window_dimensions.width)); // *scale.x;
	float height_ratio = (height / static_cast<float>(meep_window_dimensions.height));// *scale.y;

	float rotation = m_circle_test_engine.getRotation();

	glm::mat2 rotation_matrix = rotationMat2FromAngle(rotation);
	glm::vec2 rect_position(relative.x * width_ratio + view_bound.left, relative.y * height_ratio + view_bound.top);
	glm::vec2 true_position = rotation_matrix*rect_position;
	return MapPosition(true_position.x, true_position.y);
}

WindowPosition AppEngine::getMouse() const {
	return m_mouse;
}

float AppEngine::getFPS() const {
	return m_fps;
}

std::vector<TextInput>& AppEngine::getTextInputs(){
	return m_text_inputs;
}

std::vector<NumberInput>& AppEngine::getNumberInputs(){
	return m_number_inputs;
}

std::vector<Button>& AppEngine::getButtons() {
	return m_buttons;
}

/*NumberInput& AppEngine::getNumberInput(unsigned int i) {
	return m_number_inputs[i];
}*/


void AppEngine::setupWindows() {
	m_meep_window.setDimensions(100, 100);
	m_meep_window.setPosition(5, 5);
	m_meep_window.setBackgroundColour(Colour::fromInt(50, 40, 255));

	m_circle_test_window.setDimensions(
		AppEngine::s_window_dimensions.width - 145,
		AppEngine::s_window_dimensions.height - 145
	);
	m_circle_test_window.setPosition(
		120, 50
	);
	m_circle_test_window.setBackgroundColour(Colour::fromInt(50, 255, 50));

	m_information_window.setDimensions(200, 100);
	m_information_window.setPosition(200, AppEngine::s_window_dimensions.height - 150);
	m_information_window.setBackgroundColour(Colour::fromInt(255, 50, 50));
}

void AppEngine::setupTextInputs() {
	/*
	NumberInput ni1(50, 300, 100, 25);
	ni1.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		std::vector<NumberInput>& inputs = app->getNumberInputs();
		NumberInput* ni = static_cast<NumberInput*>(ti);
		ni->setPosition(ni->getValueFloat(), ni->getY());
		std::cout << ni->getValueInt() << std::endl;
		}, this);
	m_number_inputs.push_back(ni1);
	*/
	
	NumberInput ni2(50, 340, 100, 25);
	ni2.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		NumberInput* ni = static_cast<NumberInput*>(ti);
		CircleTestEngine& eng = app->getCircleTestEngine();
		eng.shift_line.x1 = ni->getValueInt();
		eng.updateTestLine();
		/*
		const Dimensions& dims = app->getCircleTestWindow().getDimensions();
		app->setCircleTestWindowSize(ni->getValueInt(), dims.height);
		app->getCircleTestEngine().refreshView();
		*/
	}, this);
	m_number_inputs.push_back(ni2);

	NumberInput ni3(50, 380, 100, 25);
	ni3.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		NumberInput* ni = static_cast<NumberInput*>(ti);
		CircleTestEngine& eng = app->getCircleTestEngine();
		eng.shift_line.y1 = ni->getValueInt();
		eng.updateTestLine();
		/*
		const Dimensions& dims = app->getCircleTestWindow().getDimensions();
		app->setCircleTestWindowSize(dims.width, ni->getValueInt());
		app->getCircleTestEngine().refreshView();
		*/
	}, this);
	m_number_inputs.push_back(ni3);

	NumberInput ni4(50, 160, 100, 25);
	ni4.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		NumberInput* ni = static_cast<NumberInput*>(ti);
		CircleTestEngine& eng = app->getCircleTestEngine();
		eng.shift_line.x2 = ni->getValueInt();
		eng.updateTestLine();
		/*
		const WindowPosition& position = app->getCircleTestWindow().getPosition();
		app->setCircleTestWindowPosition(ni->getValueInt(), position.y);
		app->getCircleTestEngine().refreshView();
		*/
	}, this);
	m_number_inputs.push_back(ni4);

	NumberInput ni5(50, 200, 100, 25);
	ni5.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		NumberInput* ni = static_cast<NumberInput*>(ti);
		CircleTestEngine& eng = app->getCircleTestEngine();
		eng.shift_line.y2 = ni->getValueInt();
		eng.updateTestLine();
		/*
		const WindowPosition& position = app->getCircleTestWindow().getPosition();
		app->setCircleTestWindowPosition(position.x, ni->getValueInt());
		app->getCircleTestEngine().refreshView();
		*/
	}, this);
	m_number_inputs.push_back(ni5);

	TextInput ti1(50, 420, 100, 30);
	ti1.setOnChange([](void* ptr, TextInput* ti) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
	}, this);
	ti1.setText("Hello");
	m_text_inputs.push_back(ti1);
}

void AppEngine::setupButtons() {
	Button b1(10.0f, 40.0f, 80.0f, 50.0f);
	b1.setLabel("massssssssive one");
	//b1.setLabel("smol");
	b1.setHoverIn([](void* ptr, Button* button) {
		std::cout << "Hello" << '\n';
	});
	b1.setButtonPressed([](void* ptr, Button* button) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		std::cout << app->getFPS() << '\n';
	}, this);
	m_buttons.push_back(b1);

	Button b2(20.0f, 400.0f, 50.0f, 40.0f);
	b2.setLabel("Step");
	b2.setButtonPressed([](void* ptr, Button* button) {
		AppEngine* app = static_cast<AppEngine*>(ptr);
		app->getCircleTestEngine().simStep(0.16);
	}, this);
	m_buttons.push_back(b2);
}

std::optional<MapPosition> AppEngine::getWindowEngineMouse(const InterfaceWindow& iw, Engine2D& engine) const {
	if (!iw.isInside(m_mouse)) {
		return std::nullopt;
	}
	WindowPosition relative = iw.getRelativePosition(m_mouse);

	Dimensions meep_window_dimensions = iw.getDimensions();

	float width = engine.getWidth();
	float height = engine.getHeight();

	float width_ratio = width / static_cast<float>(meep_window_dimensions.width);
	float height_ratio = width / static_cast<float>(meep_window_dimensions.height);

	return MapPosition(relative.x * width_ratio, relative.y * height_ratio);
}

Scale2D AppEngine::getAppToEngineScale(const InterfaceWindow& iw, const Engine2D& engine) {
	Dimensions dims = iw.getDimensions();
	float engine_width = engine.getWidth();
	float engine_height = engine.getHeight();
	return Scale2D(engine_width / dims.width, engine_height/dims.height);
}

MeepEngine::MeepEngine(): MeepEngine(0,0){};

MeepEngine::MeepEngine(float engine_width, float engine_height) : 
m_food_spawn_timer(1000),
m_width(engine_width), m_height(engine_height),
m_hovered_meep_id(std::nullopt),
m_mouse_x(0), m_mouse_y(0),
m_engine_time(0)
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
	Food* f = new Food(2);
	f->setPosition(x, y);
	m_foods[f->getId()] = f;
}

void MeepEngine::step(float dt) {
	m_engine_time += dt;

	unsigned int food_spawns = m_food_spawn_timer.add_time(dt * 1000);
	if (food_spawns > 0 && m_foods.size() < 20) {
		spawnFood();
	}
	std::vector<unsigned int> dead_m_meeps;

	//m_hovered_meep_id = std::nullopt;

	for (auto& [meep_id, meep] : m_meeps) {
		MeepStateChangeData state_change_data = meep->updateState();
		if (state_change_data.clone_meep != nullptr) {
			Meep* clone = state_change_data.clone_meep;
			clone->setPosition(meep->getX() + meep->getSize(), meep->getY() + meep->getSize());
			m_meeps[clone->getId()] = clone;
			//std::cout << "cloning" << std::endl;
		}

		MeepState meep_state = meep->getState();
		if (meep_state == MeepState::LookingForFood) {
			//meep->moveToClosestFood(m_foods);
			Meep::setTargetFood(*meep, m_foods);
		}
		setMeepVelocity(*meep);

		meep->step(dt);
		if (meep_state == MeepState::Dead) {
			dead_m_meeps.push_back(meep_id);
		}

		//check mouse hover - moved to mouseEvent
		//if (meep->collidePoint(m_mouse_x, m_mouse_y)) {
		//	m_hovered_meep_id = meep_id;
		//}

		//check collisions
		std::vector<unsigned int> collide_food_ids = meep->collideFood(m_foods, dt);
		for (unsigned int i : collide_food_ids) {
			delete m_foods[i];
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

std::map<unsigned int, Food*> MeepEngine::getFoods() const {
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

void MeepEngine::mouseEvent(const MapPosition& mouse_position) {
	m_hovered_meep_id = std::nullopt;
	for (auto& [meep_id, meep] : m_meeps) {
		bool mouse_collision = meep->isCollisionPoint(mouse_position.x, mouse_position.y);
		if (mouse_collision) {
			m_hovered_meep_id = meep_id;
		}
	}
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

void MeepEngine::setMeepVelocity(Meep& meep) {
	MapPosition* pos = meep.getTarget();
	if (pos != nullptr) {
		MapPosition curr = meep.getPosition();
		float size = meep.getSize();
		float dx = pos->x - curr.x;
		float dy = pos->y - curr.y;
		float dist = Distances::pointToPointDistanceFloat(pos->x, pos->y, curr.x, curr.y);
		if (dist < size / 2) {
			meep.setVelocity(0, 0);
			meep.setEating();
		}
		else if (dist < size) {
			//std::cout << "on food" << std::endl;
			float mx = (dx / dist) * (dist / size);
			float my = (dy / dist) * (dist / size);
			meep.setVelocity(mx, my);
			meep.setEating();
		}
		else {
			float mx = (dx / dist);
			float my = (dy / dist);
			meep.setVelocity(mx, my);
		}
	}
	else {
		meep.setVelocity(0, 0);
	}
}