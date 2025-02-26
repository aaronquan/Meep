#include "test.hpp"

TestCircle::TestCircle() : TestCircle(1) {};
TestCircle::TestCircle(float size) : Entity2D(), m_size(size), m_speed(5),
m_px(getX()), m_py(getY()), m_move_state(TestCircleState::MovingToTarget1),
m_velocity(), m_colour(0.0f, 0.0f, 0.0f)
{};

void TestCircle::updatePosition(float dt) {
	if (m_move_state != TestCircleState::Waiting) {
		m_px = getX();
		m_py = getY();
		MapPosition target = m_move_state == TestCircleState::MovingToTarget1 ? m_target1 : m_target2;
		float movement = dt * m_speed;
		float dx = target.x - getX();
		float dy = target.y - getY();
		float dist = sqrt(dx * dx + dy * dy);
		if (movement > dist) {
			setX(target.x);
			setY(target.y);
			/*
			if (m_move_state == TestCircleState::MovingToTarget1) {
				m_move_state = TestCircleState::MovingToTarget2;
			}
			else {
				m_move_state = TestCircleState::MovingToTarget1;
			}*/
		}
		else {
			float vx = dx / dist;
			float vy = dy / dist;
			//std::cout << vx << " " << vy << std::endl;
			setX(getX() + vx * movement);
			setY(getY() + vy * movement);
		}
	}
}

void TestCircle::checkTargetChange() {
	if (m_move_state == TestCircleState::MovingToTarget1 
		&& m_target1.isEqual(getX(), getY())) {
		m_move_state = TestCircleState::MovingToTarget2;
	}
	else if (m_move_state == TestCircleState::MovingToTarget2 
		&& m_target2.isEqual(getX(), getY())) {
		m_move_state = TestCircleState::MovingToTarget1;
	}
}

Vector2D TestCircle::getLastVelocity() const {
	return m_velocity;
}

void TestCircle::setLastVelocity(float dt) {
	float dx = getX() - m_px;
	float dy = getY() - m_py;
	m_velocity =  Vector2D(dx / dt, dy / dt);
}

void TestCircle::setPosition(float x, float y) {
	Entity2D::setPosition(x, y);
	//setNextPosition(MapPosition(x, y));
}

//void TestCircle::setNextPosition(MapPosition mp) {
//	m_px = mp.x; m_py = mp.y;
//}


float TestCircle::getPrevX() const {
	return m_px;
}

float TestCircle::getPrevY() const {
	return m_py;
}

Vector2D TestCircle::getLastMovement() const {
	return Vector2D(getX() - m_px, getY() - m_py);
}

float TestCircle::getSize() const {
	return m_size;
}

float TestCircle::getSpeed() const {
	return m_speed;
}

CollisionCircle TestCircle::getCollision() const {
	return CollisionCircle(getX(), getY(), getSize());
}

void TestCircle::setTarget1(float x, float y) {
	m_target1 = MapPosition(x, y);
}

void TestCircle::setTarget2(float x, float y) {
	m_target2 = MapPosition(x, y);
}

const Colour& TestCircle::getColour() const {
	return m_colour;
}

void TestCircle::setColour(Colour c) {
	m_colour = c;
}

const MapPosition* TestCircle::getCurrentTarget() const {
	if (m_move_state == TestCircleState::MovingToTarget1) {
		return &m_target1;
	}
	else if (m_move_state == TestCircleState::MovingToTarget2) {
		return &m_target2;
	}
	return nullptr;
}

CircleTestEngine::CircleTestEngine() : CircleTestEngine(20, 20) {};

CircleTestEngine::CircleTestEngine(float width, float height) :
	m_time(0.0f),
	m_width(width), m_height(height),
	m_map_collision(std::nullopt),
	m_test_line(std::nullopt),
	m_mouse(std::nullopt),
	m_view(1.0f),
	m_translate(0.0f, 0.0f),
	m_scale(1.0f, 1.0f),
	m_rotate(0.0f),
	m_view_changed(true),
	m_view_bound(0, m_width, 0, m_height),
	m_world_bound(-m_width, m_width, -m_height, m_height),
	button_test(2, 2, 4, 3),
	test_colour(0.0f, 1.0f, 0.0f),
	shift_line(0.0f, 0.0f, 1.0f, 1.0f)
{
	setup();
};

void CircleTestEngine::setup() {
	button_test.setLabel("ABC");
	button_test.setLabelScale(0.07f);
	button_test.setHoverIn([](void*, Button* button){
		button->setTextColour(Colour::fromInt(255, 255, 255));
	}, this);

	button_test.setHoverOut([](void*, Button* button) {
		button->setTextColour(Colour(0, 0, 0));
	}, this);

	button_test.setButtonPressed([](void* eng, Button* button) {
		CircleTestEngine* engine = static_cast<CircleTestEngine*>(eng);
		engine->test_colour.add(0.2f, 0, 0);
		button->setTextColour(Colour::fromInt(100, 100, 100));
	}, this);

	Shader::FragmentShader white = ShaderLibrary::generateFragmentShader("trip.frag");
	hover_button_shader.addFragmentShader(white);
	hover_button_shader.use();
	hover_button_shader.setVec2("u_resolution", glm::vec2(40.0f, 30.0f));

	test_line = CollisionLine(0.0f, 1.0f, Vector2D(1.0f, 1.0f));


	TestCircle c1(2);
	c1.setPosition(10, 12);
	c1.setTarget1(3, 0);
	c1.setTarget2(14, 20);
	m_circles.push_back(c1);

	TestCircle c2(3);
	c2.setPosition(5, 5);

	c2.setTarget1(2, 5);
	c2.setTarget2(18, 7);
	c2.setColour(Colour(1.0f, 1.0f, 0.5f));
	m_circles.push_back(c2);

	TestCircle c3(1);
	c3.setPosition(2, 2);
	m_circles.push_back(c3);

	NumberInput ni1(0, 50, 50, 25, 1.0f, false);
	ni1.setOnChange([](void* ptr, TextInput* ti) {
		CircleTestEngine* engine = static_cast<CircleTestEngine*>(ptr);
		
		std::vector<TestCircle>& circles = engine->getCircles();
		TestCircle& circle = circles[0];

		NumberInput* ni = static_cast<NumberInput*>(ti);
		circle.setX(ni->getValueFloat());
	}, this);
	m_number_inputs.push_back(ni1);
	NumberInput ni2(0, 100, 50, 25, 1.0f, true);
	ni2.setOnChange([](void* ptr, TextInput* ti) {
		CircleTestEngine* engine = static_cast<CircleTestEngine*>(ptr);
		std::vector<TestCircle>& circles = engine->getCircles();
		TestCircle& circle = circles[0];
		NumberInput* ni = static_cast<NumberInput*>(ti);
		circle.setY(ni->getValueFloat());
	}, this);
	m_number_inputs.push_back(ni2);
}

void CircleTestEngine::step(float dt) {
	m_time += dt;
	hover_button_shader.use();
	hover_button_shader.setFloat("u_time", m_time);
	/*
	if (m_circles.size() >= 2) {
		TestCircle& c1 = m_circles[0];
		
		c1.updatePosition(dt);

		TestCircle& c2 = m_circles[1];

		c2.updatePosition(dt);

		handleCollision(c1, c2);

		c1.setLastVelocity(dt);
		c2.setLastVelocity(dt);
	}*/

	//m_mouse inside rect
	if (m_mouse.has_value()) {
		//Colour regular_colour(200, 100, 100);
		//Colour hover_colour(100, 100, 100);
		const MapPosition& mouse = m_mouse.value();
		for (TestCircle& circle : m_circles) {
			CollisionCircle cc = circle.getCollision();
			if (cc.collidesPoint(mouse.x, mouse.y)) {
				//circle.setColour(regular_colour);
			}
			else {
				//circle.setColour(hover_colour);
			}
		}
		/*
		LineSide side = test_line.sideOfPoint(mouse.x, mouse.y);
		if (side == LineSide::Left) {
			test_string = "Left";
		}
		else if (side == LineSide::Right) {
			test_string = "Right";
		}
		else {
			test_string = "Un";
		}*/

		//float v = test_line.pointEquationValue(mouse.x, mouse.y);
		QuadraticSolution qs = solveQuadratic(2, 4, -16);
		std::string qss = quadraticSolutionToString(qs);
		//std::cout << v << ' ' << mouse.x << ' ' << mouse.y << '\n';
		//test_string = std::to_string(v);
		test_string = qss;
	}


	m_last_dt = dt;
}

void CircleTestEngine::simStep(float dt) {
	test_circles.clear();
	m_map_collisions.clear();
	intersect_circles2.clear();
	for (const TestCircle& circle : m_circles) {
		//CircleData cd(circle.getX(), circle.getY(), circle.getLastVelocity().getLength() * dt);
		//test_circles.push_back(cd);
	}

	m_map_collisions.push_back(MapPosition(1.0f, 2.0f));

	if (m_circles.size() >= 2) {
		TestCircle& c1 = m_circles[0];
		TestCircle& c2 = m_circles[1];

		c1.updatePosition(dt);


		c2.updatePosition(dt); // this moves out of the way

		float radius_add = c1.getSize() + c2.getSize();
		test_circles.push_back(CircleData(c1.getX(), c1.getY(), radius_add));
		test_circles.push_back(CircleData(c2.getPrevX(), c2.getPrevY(), c2.getSpeed() * dt));

		CollisionCircle collision_circle1 = c1.getCollision();
		CollisionCircle collision_circle2 = c2.getCollision();


		if (collision_circle1.collidesCircle(collision_circle2)) {
			const MapPosition* goal = c2.getCurrentTarget();
			if (goal != nullptr) {
				CollisionCircle no_go(c1.getX(), c1.getY(), radius_add);
				CollisionCircle possible_centres(c2.getPrevX(), c2.getPrevY(), c2.getSpeed()*dt);

				//CircleToCircleCollisionDetails ccd = CollisionCircle::collisionCircleDetails(no_go, possible_centres);
				//std::vector<MapPosition> points;
				//= CollisionCircle::collisionCirclePoints(ccd);

				if (possible_centres.collidesPoint(goal->x, goal->y) && !no_go.collidesPoint(goal->x, goal->y)) {
					//move to goal
					c2.setX(goal->x);
					c2.setY(goal->y);
				}
				else {
					CircleToCircleCollisionDetails ccd = CollisionCircle::collisionCircleDetails(no_go, possible_centres);
					std::vector<MapPosition> points = CollisionCircle::collisionCirclePoints(ccd);
					CollisionLine cl = CollisionLine::from2Points(goal->x, goal->y, c2.getPrevX(), c2.getPrevY());

					LineCircleCollisionDetails lccd_possible_centres = CollisionLine::collisionLineToCircleDetails(cl, possible_centres);
					//must intersect (as center of possible_centres is a point in cl)
					if (lccd_possible_centres.type == LineCircleCollisionType::Intersecting) {
						if (!no_go.collidesPoint(lccd_possible_centres.p1.x, lccd_possible_centres.p1.y)) {
							points.push_back(MapPosition(lccd_possible_centres.p1.x, lccd_possible_centres.p1.y));
						}
						else if (!no_go.collidesPoint(lccd_possible_centres.p2.x, lccd_possible_centres.p2.y)) {
							points.push_back(MapPosition(lccd_possible_centres.p2.x, lccd_possible_centres.p2.y));
						}
					}
					else {
						std::cout << "Shouldn't be possible" << '\n';
					}

					LineCircleCollisionDetails lccd_no_go = CollisionLine::collisionLineToCircleDetails(cl, no_go);
					if (lccd_no_go.type == LineCircleCollisionType::Intersecting) {
						if (possible_centres.collidesPoint(lccd_no_go.p1.x, lccd_no_go.p1.y)) {
							points.push_back(MapPosition(lccd_no_go.p1.x, lccd_no_go.p1.y));
						}
						if (possible_centres.collidesPoint(lccd_no_go.p2.x, lccd_no_go.p2.y)) {
							points.push_back(MapPosition(lccd_no_go.p2.x, lccd_no_go.p2.y));
						}
					}
					else if (lccd_no_go.type == LineCircleCollisionType::Touching) {
						if (possible_centres.collidesPoint(lccd_no_go.p1.x, lccd_no_go.p1.y)) {
							points.push_back(MapPosition(lccd_no_go.p1.x, lccd_no_go.p1.y));
						}
					}
					else {
						std::cout << "Not touching no_go" << '\n';
					}
					float min_distance = std::numeric_limits<float>::max();
					MapPosition min_distance_point(0.0f, 0.0f);
					for (const MapPosition& pos : points) {
						float dist = Distances::pointToPointDistanceSquaredFloat(pos.x, pos.y, goal->x, goal->y);
						if (dist < min_distance) {
							min_distance = dist;
							min_distance_point = MapPosition(pos.x, pos.y);
						}
						intersect_circles2.push_back(CircleData(pos.x, pos.y, 0.3f));
					}
					c2.setX(min_distance_point.x);
					c2.setY(min_distance_point.y);
				}
			}

		}

		//CollisionCircle coll1(c2.getPrevX(), c2.getPrevY(), c2.);


		handleCollision(c1, c2);

		c1.setLastVelocity(dt);
		c2.setLastVelocity(dt);

		c1.checkTargetChange();
		c2.checkTargetChange();
	}



	std::cout << "Step: " << dt << '\n';
}

MapPosition CircleTestEngine::collisionCircleAlgo1(const TestCircle& solid_circle, const TestCircle& moving_circle, float dt) {
	CollisionCircle collision_solid_circle = solid_circle.getCollision();
	CollisionCircle collision_moving_circle = moving_circle.getCollision();

	float radius_add = solid_circle.getSize() + moving_circle.getSize();
	if (collision_solid_circle.collidesCircle(collision_moving_circle)) {
		const MapPosition* goal = moving_circle.getCurrentTarget();
		if (goal != nullptr) {
			CollisionCircle no_go(solid_circle.getX(), solid_circle.getY(), radius_add);
			CollisionCircle possible_centres(moving_circle.getPrevX(), moving_circle.getPrevY(), moving_circle.getSpeed() * dt);

			if (possible_centres.collidesPoint(goal->x, goal->y) && !no_go.collidesPoint(goal->x, goal->y)) {
				//move to goal
				return *goal;
			}
			CircleToCircleCollisionDetails ccd = CollisionCircle::collisionCircleDetails(no_go, possible_centres);
			std::vector<MapPosition> points = CollisionCircle::collisionCirclePoints(ccd);
			CollisionLine cl = CollisionLine::from2Points(goal->x, goal->y, moving_circle.getPrevX(), moving_circle.getPrevY());

			LineCircleCollisionDetails lccd_possible_centres = CollisionLine::collisionLineToCircleDetails(cl, possible_centres);
			//must intersect (as center of possible_centres is a point in cl)
			if (lccd_possible_centres.type == LineCircleCollisionType::Intersecting) {
				if (!no_go.collidesPoint(lccd_possible_centres.p1.x, lccd_possible_centres.p1.y)) {
					points.push_back(MapPosition(lccd_possible_centres.p1.x, lccd_possible_centres.p1.y));
				}
				else if (!no_go.collidesPoint(lccd_possible_centres.p2.x, lccd_possible_centres.p2.y)) {
					points.push_back(MapPosition(lccd_possible_centres.p2.x, lccd_possible_centres.p2.y));
				}
			}
			else {
				std::cout << "Shouldn't be possible" << '\n';
			}
		}
	}
}

 void CircleTestEngine::handleCollision(TestCircle& c1, TestCircle& c2) {
	CollisionCircle collision_circle1 = c1.getCollision();
	CollisionCircle collision_circle2 = c2.getCollision();
	std::optional<CircleToCircleCollisionDetails> deets_opt = collision_circle1.collidesCircleDetails(collision_circle2);
	if (deets_opt.has_value()) {
		CircleToCircleCollisionDetails circle_collision_details = deets_opt.value();
		std::vector<MapPosition> points = CollisionCircle::collisionCirclePoints(circle_collision_details);
		m_map_collisions.insert(std::end(m_map_collisions), std::begin(points), std::end(points));
		//m_map_collisions = points;
	}
	
	/*
	//regular push collision
	if (deets != std::nullopt) {
		Vector2D opposite_direction = deets->direction.opposite();

		float half_distance = deets->distance / 2;
		MapPosition mid = MapPosition(c1.getX(), c1.getY());
		Vector2D to_mid = deets->direction * half_distance;
		mid = mid + to_mid;
		m_map_collision = mid;

		float radius_distance = c1.getSize() + c2.getSize();
		float move_distance = deets->distance - radius_distance;
		float half_move_distance = move_distance / 2;

		Vector2D move_c1 = deets->direction * half_move_distance;
		Vector2D move_c2 = opposite_direction * half_move_distance;
		c1.setPosition(c1.getX() + move_c1.getX(), c1.getY() + move_c1.getY());
		c2.setPosition(c2.getX() + move_c2.getX(), c2.getY() + move_c2.getY());
	}
	else {
		m_map_collision = std::nullopt;
	}
	*/
	//m_map_collisions.clear();

	/*
	//fancy collision 1
	if (deets != std::nullopt) {
		Vector2D v1 = c1.getLastVelocity();
		Vector2D v2 = c2.getLastVelocity();

		float r1 = c1.getSize();
		float r2 = c2.getSize();
		float x1 = c1.getX(); float y1 = c1.getY();
		float x2 = c2.getX(); float y2 = c2.getY();

		float radius_diff = abs(r1 - r2);

		float dx = x2 - x1; float dy = y2 - y1;
		float dist = deets->distance;
		float dist_squared = deets->distance_squared;

		float r1_squared = r1 * r1;
		float r2_squared = r2 * r2;
		float r1r2_diff = r1_squared - r2_squared;

		float a = r1r2_diff / (2 * dist_squared);
		float b1 = (2 * (r1_squared + r2_squared)) / dist_squared;
		float b2 = (r1r2_diff * r1r2_diff) / (dist_squared * dist_squared);
		float b = sqrt(b1 - b2 - 1);

		float xa = ((x1 + x2) / 2) + a * dx;
		float xb = (b * (y2 - y1)) / 2;
		float cx1 = xa + xb;
		float cx2 = xa - xb;

		float ya = ((y1 + y2) / 2) + a * dy;
		float yb = (b * (x1 - x2)) / 2;
		float cy1 = ya + yb;
		float cy2 = ya - yb;
		MapPosition coll1;
		coll1.x = cx1;
		coll1.y = cy1;
		MapPosition coll2;
		coll2.x = cx2;
		coll2.y = cy2;

		m_map_collisions.push_back(coll1);
		m_map_collisions.push_back(coll2);
	}
	else {
		m_map_collision = std::nullopt;
	}
	*/
}

 float CircleTestEngine::getLastDeltaTime() const {
	 return m_last_dt;
 }

std::optional<MapPosition> CircleTestEngine::getCollision() const {
	return m_map_collision;
}

std::optional<float> CircleTestEngine::getTestLine() const {
	return 0;
}

const std::vector<MapPosition>& CircleTestEngine::getCollisions()  const {
	return m_map_collisions;
}

const std::vector<TestCircle>& CircleTestEngine::getCirclesConst() const {
	return m_circles;
}

std::vector<TestCircle>& CircleTestEngine::getCircles(){
	return m_circles;
}

std::vector<NumberInput>& CircleTestEngine::getNumberInputs(){
	return m_number_inputs;
}

const std::vector<NumberInput>& CircleTestEngine::getNumberInputs() const {
	return m_number_inputs;
}

std::vector<TextInput>& CircleTestEngine::getTextInputs(){
	return m_text_inputs;
}

const glm::mat4& CircleTestEngine::getView() const {
	return m_view;
}

bool CircleTestEngine::getHasViewChanged() const {
	return m_view_changed;
}

void CircleTestEngine::refreshView() {

	//testing line collision with view
	updateDisplayLine();

	m_view_changed = true;
}

void CircleTestEngine::appliedViewChanges() {
	m_view_changed = false;
 }

const glm::vec2& CircleTestEngine::getTranslate() const {
	return m_translate;
}

const glm::vec2& CircleTestEngine::getScale() const {
	return m_scale;
}

float CircleTestEngine::getRotation() const {
	return m_rotate;
}

const RectBound& CircleTestEngine::getWorldBound() const {
	return m_world_bound;
}

const RectBound& CircleTestEngine::getViewBound() const {
	/*
	float left = m_translate.x * m_scale.x;
	float right = (m_width * m_scale.x) + m_translate.x;
	view_bound.left = left;
	view_bound.right = right;
	//std::cout << left << ' ' << right << '\n';
	*/
	return m_view_bound;
}

const std::optional<MapPosition> CircleTestEngine::getMouse() const {
	return m_mouse;
}

float CircleTestEngine::getWidth() const {
	return m_width;
}

float CircleTestEngine::getHeight() const {
	return m_height;
}

bool CircleTestEngine::mouseClickTextCollision(const WindowPosition& mouse){
	for (TextInput& ti : m_number_inputs) {
		std::optional<Vector2D> collision = ti.collisionPoint(mouse.x, mouse.y);
		if (collision.has_value()) {
			TextInput::setHighlightedInput(&ti);
			return true;
		}
	}
	return false;
}

void CircleTestEngine::mouseMoveEvent(std::optional<MapPosition> mouse) {
	/*
	m_map_collisions.clear();
	for (TestCircle& circle: m_circles) {
		
		if (circle.getCollision().collidesPoint(mouse.x, mouse.y)) {
			circle.setColour(Colour(255, 0, 0));
			for (TestCircle& other_circle : m_circles) {
				if (&circle != &other_circle) {
					const CircleToCircleCollisionDetails circ = circle.getCollision().collidesCircleDetails(other_circle.getCollision());
					for (MapPosition& mp : CollisionCircle::collisionCirclePoints(circ)) {
						m_map_collisions.push_back(mp);
					}
				}
			}
		}
		else {
			circle.setColour(Colour(0, 0, 0));
		}
	}
	std::cout << mouse.x << ' ' << mouse.y << '\n';
	*/

	if (mouse.has_value()) {
		MapPosition& pos = mouse.value();
		button_test.mouseMoveEvent(pos.x, pos.y);
	}
	m_mouse = mouse;
}

void CircleTestEngine::mousePressEvent() {
	if (m_mouse.has_value()) {
		MapPosition& mouse = m_mouse.value();
		for (TestCircle& circle : m_circles) {
			if (circle.getCollision().collidesPoint(mouse.x, mouse.y)) {
				circle.setColour(Colour(255, 0, 255));
			}
		}

		button_test.mouseDownEvent();
	}

}

void CircleTestEngine::mouseReleaseEvent() {
	if (m_mouse.has_value()) {
		MapPosition& mouse = m_mouse.value();
		for (TestCircle& circle : m_circles) {
			if (!circle.getCollision().collidesPoint(mouse.x, mouse.y)) {
				circle.setColour(Colour(255, 0, 0));
			}
		}
	}
}

void CircleTestEngine::onKey(int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS) {
		
	}
}

void CircleTestEngine::refreshViewBound() {
	float left = m_translate.x;
	float right = (m_width * m_scale.x) + m_translate.x;
	m_view_bound.left = left;
	m_view_bound.right = right;

	float top = m_translate.y;
	float bottom = (m_height * m_scale.y) + m_translate.y;
	m_view_bound.top = top;
	m_view_bound.bottom = bottom;
}

void CircleTestEngine::translateViewBoundX(float x) {
	float shift = x;
	if (x > 0){
		if (m_view_bound.right + x > m_world_bound.right) {
			shift = m_world_bound.right - m_view_bound.right;
		}
	}
	else{
		if (m_view_bound.left + x < m_world_bound.left) {
			shift = m_world_bound.left - m_view_bound.left;
		}
	}
	std::cout << shift << '\n';
	m_view_bound.left += shift; 
	m_view_bound.right += shift;
}

void CircleTestEngine::translateViewBoundY(float y) {
	float shift = y;
	if (y > 0) {
		if (m_view_bound.bottom + y > m_world_bound.bottom) {
			shift = m_world_bound.bottom - m_view_bound.bottom;
		}
	}
	else {
		if (m_view_bound.top + y < m_world_bound.top) {
			shift = m_world_bound.top - m_view_bound.top;
		}
	}
	m_view_bound.top += shift;
	m_view_bound.bottom += shift;
}

void CircleTestEngine::scaleViewBound(float factor) {
	if (factor > 1.0f) {
		//zoom out
		float world_width = m_world_bound.getWidth();
		float view_width = m_view_bound.getWidth();
		float width_ratio = world_width / view_width;
		float world_height = m_world_bound.getHeight();
		float view_height = m_view_bound.getHeight();
		float height_ratio = world_height / view_height;
		float new_factor = factor;
		if (new_factor > width_ratio) {
			new_factor = width_ratio;
		}
		if(new_factor > height_ratio){
			new_factor = height_ratio;
		}
		std::cout << "Ratios: " << width_ratio << ' ' << height_ratio << '\n';
		std::cout << "Factor: " << new_factor << '\n';
		float diff = (new_factor - 1);

		float increase_x = diff * view_width;
		float half_increase_x = increase_x / 2;

		float new_view_width = view_width + increase_x;

		if (m_view_bound.left - half_increase_x < m_world_bound.left) {

			m_view_bound.left = m_world_bound.left;
			m_view_bound.right = m_world_bound.left + new_view_width;
		}
		else if (m_view_bound.right + half_increase_x > m_world_bound.right) {
			m_view_bound.right = m_world_bound.right;
			m_view_bound.left = m_world_bound.right - new_view_width;
		}
		else {
			m_view_bound.left -= half_increase_x;
			m_view_bound.right += half_increase_x;
		}

		float increase_y = diff * view_height;
		float half_increase_y = increase_y / 2;

		float new_view_height = view_height + increase_y;

		if (m_view_bound.top - half_increase_y < m_world_bound.top) {
			m_view_bound.top = m_world_bound.top;
			m_view_bound.bottom = m_world_bound.top + new_view_height;
		}
		else if (m_view_bound.bottom + half_increase_y > m_world_bound.bottom) {
			m_view_bound.bottom = m_world_bound.bottom;
			m_view_bound.top = m_world_bound.bottom - new_view_height;
		}
		else {
			m_view_bound.top -= half_increase_y;
			m_view_bound.bottom += half_increase_y;
		}

	}
	else if (factor > 0.0f){
		//zoom in
		float diff = (1 - factor);
		float half_reduction_x = (diff * m_view_bound.getWidth()) / 2;
		m_view_bound.left += half_reduction_x;
		m_view_bound.right -= half_reduction_x;

		float half_reduction_y = (diff * m_view_bound.getHeight()) / 2;
		m_view_bound.top += half_reduction_y;
		m_view_bound.bottom -= half_reduction_y;
	}
}

void CircleTestEngine::rotateClockwise90() {
	m_rotate += acos(0.0);
	//m_view
}

//void CircleTestEngine::setMaxZoomOut

//void CircleTestEngine::printViewBound() {
//	m_view_bound.easyPrint();
//}

void CircleTestEngine::keyboardPressEvent(const char* key_name) {
	std::cout << *key_name << '\n';
	m_view_changed = true;
	switch (*key_name) {
	case 'a':
		translateViewBoundX(-0.5);
		//m_translate.x -= 0.5;
		//m_view = AppEngine::getEngineView(window, width, height, 1.0, translate.x, translate.y);;
		break;
	case 's':
		translateViewBoundY(0.5);
		//m_view = glm::translate(m_view, glm::vec3(0, 0.5, 0));
		break;
	case 'd':
		translateViewBoundX(0.5);
		//m_view = glm::translate(m_view, glm::vec3(0.5, 0, 0));
		break;
	case 'w':
		translateViewBoundY(-0.5);
		//m_translate.y -= 0.5;
		break;
	case 'q':
		//m_scale += 0.2;
		//if (m_scale.x > 1) {
		//	m_scale.x = 1;
		//}
		scaleViewBound(1.2);
		break;
	case 'e':
		//m_scale -= 0.2;
		//if (m_scale.x < 0.2) {
		//	m_scale.x = 0.2;
		//}
		scaleViewBound(0.8);
		break;
	case 'r':
		//rotate 90
		rotateClockwise90();
		break;
	case 't':
		//testing function
		
		m_view_changed = false;
		break;
	default:
		m_view_changed = false;
	}
	if (m_view_changed) {
		refreshView();
		//m_view_bound.easyPrint();
	}

	if (*key_name == '-') {
		//m_view = glm::scale(m_view)
	}
	else if (*key_name == '+') {

	}
}


void CircleTestEngine::updateTestLine() {
	intersect_circles.clear();
	test_line = CollisionLine::from2Points(shift_line.x1, shift_line.y1, shift_line.x2, shift_line.y2);
	LineCircleCollisionDetails collision_details = CollisionLine::collisionLineToCircleDetails(test_line, m_circles[1].getCollision());
	if (collision_details.type == LineCircleCollisionType::Intersecting) {
		intersect_circles.push_back(CircleData(collision_details.p1.x, collision_details.p1.y, 1.0f));
		intersect_circles.push_back(CircleData(collision_details.p2.x, collision_details.p2.y, 1.0f));
	}
	std::stringstream ss;
	ss << lineCircleCollisionTypeToString(collision_details.type) << ' ' << collision_details.p1.toString() << ' ' << collision_details.p2.toString();
	debug_text = ss.str();
	updateDisplayLine();
}

void CircleTestEngine::updateDisplayLine() {
	CollisionRect collision_view_bound = CollisionRect::fromRectBound(m_view_bound);
	LineRectCollisionDetails collision_details = test_line.collisionLineToRectDetails(test_line, collision_view_bound);
	if (collision_details.type == LineRectCollisionType::Intersecting) {
		display_line = LineSegment(collision_details.p1.x, collision_details.p1.y,
			collision_details.p2.x, collision_details.p2.y);
	}
	else {
		display_line = LineSegment();
	}

	//debug_text = lineRectCollisionTypeToString(collision_details.type);
	//std::cout << debug_text << '\n';
}
