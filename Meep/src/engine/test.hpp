#pragma once
#include <optional>
#include <iostream>

#include "../math/vector2d.hpp"
#include "../meep/entity.hpp"
#include "../shapes/shapes.hpp"

#include "../math/map.hpp"
#include "../text/text_input.hpp"


#include "../window/window.hpp"
#include "../ui_elements/button.hpp"

#include "general_engines.hpp"

#include "../math/geometry.hpp"
#include "../math/distances.hpp"


enum class TestCircleState {
	Waiting, MovingToTarget1, MovingToTarget2
};

struct CircleData {
	float x;
	float y;
	float radius;
	CircleData(float _x, float _y, float r) : x(_x), y(_y), radius(r) {};
};

class TestCircle: public Entity2D {
public:
	TestCircle();
	TestCircle(float size);

	void updatePosition(float dt);

	void setPosition(float x, float y);
	void checkTargetChange();
	//void setNextPosition(MapPosition mp);

	float getPrevX() const;
	float getPrevY() const;
	Vector2D getLastMovement() const;
	float getSize() const;
	float getSpeed() const;

	CollisionCircle getCollision() const;

	Vector2D getLastVelocity() const;
	void setLastVelocity(float dt);

	void setTarget1(float x, float y);
	void setTarget2(float x, float y);

	const Colour& getColour() const;
	void setColour(Colour c);

	const MapPosition* getCurrentTarget() const;
private:
	float m_px;
	float m_py;
	float m_size;
	float m_speed;
	Colour m_colour;
	Vector2D m_velocity;

	TestCircleState m_move_state;
	MapPosition m_target1;
	MapPosition m_target2;

};

class CircleTestEngine : public Engine2DWorldViewBound {
public:
	CircleTestEngine();
	CircleTestEngine(float width, float height);
	void setup();
	void step(float dt);//live time step
	void simStep(float dt); //step for circles with v only

	float getWidth() const;
	float getHeight() const;
	float getLastDeltaTime() const;
	std::optional<MapPosition> getCollision() const;
	std::optional<float> getTestLine() const;

	const std::vector<MapPosition>& getCollisions() const;
	const std::vector<TestCircle>& getCirclesConst() const;

	std::vector<TestCircle>& getCircles();
	std::vector<NumberInput>& getNumberInputs();
	const std::vector<NumberInput>& getNumberInputs() const;
	std::vector<TextInput>& getTextInputs();

	const glm::mat4& getView() const;
	bool getHasViewChanged() const;
	void refreshView();
	void appliedViewChanges();

	const glm::vec2& getTranslate() const;
	const glm::vec2& getScale() const;
	float getRotation() const;

	const RectBound& getWorldBound() const;
	const RectBound& getViewBound() const;

	const std::optional<MapPosition> getMouse() const;

	void mouseMoveEvent(std::optional<MapPosition> mouse);
	bool mouseClickTextCollision(const WindowPosition& mouse);
	void mousePressEvent();
	void mouseReleaseEvent();
	void onKey(int key, int scancode, int action, int mods);

	void keyboardPressEvent(const char* key_name);

	void updateTestLine();
	void updateDisplayLine();

	Button button_test;
	ShaderLibrary::MVPShader hover_button_shader;
	Colour test_colour;
	std::vector<CircleData> test_circles;
	CollisionLine test_line;
	LineSegment shift_line;
	LineSegment display_line;
	std::string debug_text;

	std::string test_string;

	std::vector<CircleData> intersect_circles;
	std::vector<CircleData> intersect_circles2;

	
private:
	void handleCollision(TestCircle& c1, TestCircle& c2);
	void refreshViewBound();
	void translateViewBoundX(float x);
	void translateViewBoundY(float y);
	void scaleViewBound(float factor);
	void rotateClockwise90();

	MapPosition collisionCircleAlgo1(const TestCircle& solid_circle, const TestCircle& moving_circle, float dt);
	//void printViewBound();

	float m_time;

	std::vector<TestCircle> m_circles;
	float m_width;
	float m_height;
	float m_last_dt;
	std::optional<MapPosition> m_map_collision;
	std::vector<MapPosition> m_map_collisions;
	std::optional<float> m_test_line;

	std::vector<NumberInput> m_number_inputs;
	std::vector<TextInput> m_text_inputs;

	std::optional<MapPosition> m_mouse; // in engine coordinates

	glm::mat4 m_view;
	bool m_view_changed;
	glm::vec2 m_translate;
	glm::vec2 m_scale;
	float m_rotate;

	RectBound m_view_bound;
	RectBound m_world_bound;


};