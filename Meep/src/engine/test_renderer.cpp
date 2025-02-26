#include "test_renderer.hpp"
#include "../debug/prints.hpp"

CircleTestEngineRenderer::CircleTestEngineRenderer(): 
	//m_view(1.0f), 
	m_projection(1.0f),
	m_mvp(1.0f)
	//m_view_change(true) 
{
	setupShaders();
};

void CircleTestEngineRenderer::setupShaders() {
	Shader::FragmentShader circle = ShaderLibrary::generateFragmentShader("circle.frag");
	m_circle_shader.addFragmentShader(circle);
	glm::mat4 identity(1.0f);
	m_circle_shader.use();
	m_circle_shader.setMVP(identity);
	//m_circle_shader.setModel(identity);
	//m_circle_shader.setView(identity);
	//m_circle_shader.setProjection(identity);
	m_circle_shader_old.addFragmentShader(circle);
	m_circle_shader_old.use();
	m_circle_shader_old.setModel(identity);
	m_circle_shader_old.setView(identity);
	m_circle_shader_old.setProjection(identity);

	Shader::FragmentShader colour = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(colour);
	m_colour_shader.use();
	m_colour_shader.setMVP(identity);
}

void CircleTestEngineRenderer::setShaderViewProjection(CircleTestEngine& engine, const InterfaceWindow& window) {
	if (!engine.getHasViewChanged()) return;
	//if (!m_view_change) return;
	float width = engine.getWidth();
	float height = engine.getHeight();
	//glm::vec2 translate = engine.getTranslate();
	const RectBound& view_bound = engine.getViewBound();
	view_bound.easyPrint();
	glm::vec2 scale = engine.getScale();
	//glm::mat4 view = engine.getView();
	//glm::mat4 view = AppEngine::getEngineView(window, 
	// view_bound.getWidth(), view_bound.getHeight(), 1.0f, 
	// view_bound.left, view_bound.top);
	glm::mat4 view = AppEngine::getEngineViewFromBound(window, engine);
	view = glm::rotate(view, engine.getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::mat4 view(1.0f);
	
	/*glm::mat4 projection = glm::ortho(
		0.0f,
		static_cast<float>(width),
		static_cast<float>(height),
		0.0f,
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::max()
	);*/
	glm::mat4 projection = glm::ortho(
		view_bound.left, view_bound.right, 
		view_bound.bottom, view_bound.top,
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::max()
	);

	//m_view = view;
	m_projection = projection;
	m_mvp = glm::mat4(1.0f);

	m_mvp *= m_projection;
	m_mvp *= view;
	m_circle_shader.use();
	m_circle_shader.setMVP(m_mvp);

	m_circle_shader_old.use();
	m_circle_shader_old.setView(view);
	m_circle_shader_old.setProjection(m_projection);

	engine.appliedViewChanges();
	//m_view_change = true;
}

void CircleTestEngineRenderer::render(CircleTestEngine& engine, const InterfaceWindow& window) {
	setShaderViewProjection(engine, window);
	const Dimensions& dims = window.getDimensions();
	const WindowPosition& window_position = window.getPosition();
	int scissor_y = AppEngine::s_window_dimensions.height - dims.height - static_cast<int>(window_position.y);
	glScissor(window_position.x, scissor_y, dims.width, dims.height); // y - accounts for y starting at bottom
	glEnable(GL_SCISSOR_TEST);
	renderGrid(engine);

	const std::vector<TestCircle>& circles = engine.getCirclesConst();
	m_circle_shader.use();
	//m_circle_shader.setVec3("colour", glm::vec3(255, 0, 0));
	for (const TestCircle& circle : circles) {
		glm::mat4 model(1.0f);
		float size = circle.getSize()*2;
		model = glm::translate(model, glm::vec3(circle.getX(), circle.getY(), 0));
		model = glm::scale(model, glm::vec3(size, size, 1));
		//m_circle_shader_old.use();
		//m_circle_shader_old.setModel(model);
		glm::mat4 mvp = getMVP(model);
		//m_circle_shader.use();
		m_circle_shader.setMVP(mvp);
		m_circle_shader.setVec3("colour", circle.getColour().toVec3());
		//printVec3(circle.getColour().toVec3());
		Quad::draw();
	}

	/*
	m_circle_shader.setVec3("colour", glm::vec3(0, 0, 255));
	glm::mat4 test_model(1.0f);
	test_model = glm::translate(test_model, glm::vec3(5, 5, 0));
	Scale2D sc = AppEngine::getAppToEngineScale(window, engine);
	test_model = glm::scale(test_model, glm::vec3(4*sc.x, 4*sc.y, 0));
	//m_circle_shader.setModel(test_model);
	glm::mat4 mvp = getMVP(test_model);
	m_circle_shader.setMVP(mvp);
	Quad::draw();
	*/

	std::vector<MapPosition> collisions = engine.getCollisions();
	m_circle_shader.use();
	m_circle_shader.setVec3("colour", glm::vec3(0, 255, 255));
	for (MapPosition& position: collisions) {
		float size = 0.5;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, size, 1.0f));
		//m_circle_shader.setModel(model);
		glm::mat4 mvp = getMVP(model);
		m_circle_shader.setMVP(mvp);
		Quad::draw();
	}

	for (const TestCircle& circle : circles) {
		Vector2D movement = circle.getLastVelocity();
		//movement = movement.opposite();
		if (!movement.isZero()) {
			float x1 = circle.getX();
			float y1 = circle.getY();
			float scale = 1;
			LineDrawer::draw(x1, y1, x1 + movement.getX() * scale, y1 + movement.getY() * scale, m_mvp);
		}
	}

	//render number inputs
	bool any_highlighted = false;
	m_colour_shader.use();
	m_colour_shader.setVec3("colour", glm::vec3(0.4, 0.3, 0.7));
	for (const NumberInput& input : engine.getNumberInputs()) {
		if (input.isHighlighted()) {
			any_highlighted = true;
			m_colour_shader.use();
			m_colour_shader.setVec3("colour", Colour::fromInt(200, 100, 40).toVec3());
		}
		else {
			m_colour_shader.use();
			m_colour_shader.setVec3("colour", glm::vec3(0.4, 0.3, 0.7));
		}
		input.draw(m_colour_shader);
	}
	if (any_highlighted) {
		m_colour_shader.use();
		m_colour_shader.setVec3("colour", Colour(0, 0, 0).toVec3());
		TextInput::drawHighlightedCursor(m_colour_shader);
	}

	glm::vec3 text_scale = glm::vec3(0.05f, 0.05f, 1.0f);


	glm::mat4 text_mvp = m_mvp;
	glm::mat4 text_model(1.0f);
	text_model = glm::translate(text_model, glm::vec3(6.0f, 3.0f, 0.0f));
	text_model = glm::scale(text_model, text_scale);
	text_mvp *= text_model;

	// needs fixing
	LineDrawer::setColour(Colour::fromInt(100, 20, 100));
	LineSegment display_line = engine.display_line;
	LineDrawer::drawLineSegment(display_line, m_mvp);
	DrawText::renderTextMVP(engine.debug_text, text_mvp);
	//LineDrawer::draw(0, 0, 4.56, 5, m_mvp);

	glm::mat4 test_mvp = m_mvp;
	glm::mat4 test_model(1.0f);
	//test_model = glm::rotate(test_model, 1.00f, glm::vec3(0.0f, 0.0f, 1.0f));
	//printMat4(test_model);
	test_model = glm::translate(test_model, glm::vec3(6.0f, 0.0f, 0.0f));
	test_model = glm::scale(test_model, text_scale);
	//test_model = glm::translate(test_model, glm::vec3(6.0f, 0.0f, 0.0f));
	test_mvp *= test_model;
	DrawText::renderTextMVP(engine.test_string, test_mvp);

	std::optional<MapPosition> map_mouse = engine.getMouse();
	if (map_mouse.has_value()) {
		MapPosition m = map_mouse.value();
		float x = engine.test_line.getX(m.y);
		float y = engine.test_line.getY(m.x);

		glm::mat4 test_mvp = m_mvp;
		test_model = glm::mat4(1.0f);
		test_model = glm::translate(test_model, glm::vec3(6.0f, 1.0f, 0.0f));
		test_model = glm::scale(test_model, text_scale);
		test_mvp *= test_model;
		std::stringstream ss;
		ss << m.x << ' ' << m.y << " / " << x << ' ' << y;
		DrawText::renderTextMVP(ss.str(), test_mvp);
	}

	//drawing test circles
	for (const CircleData& circle : engine.test_circles) {
		LineCircleDrawer::draw(circle.x, circle.y, circle.radius, m_mvp, 40);
	}

	m_circle_shader.use();
	m_circle_shader.setVec3("colour", glm::vec3(0.9f, 0.4f, 0.2f));
	for (const CircleData& circle : engine.intersect_circles) {
		float size = 0.3;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(circle.x, circle.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, size, 1.0f));
		glm::mat4 mvp = getMVP(model);
		m_circle_shader.setMVP(mvp);
		Quad::draw();
	}

	
	m_circle_shader.setVec3("colour", glm::vec3(0.6f, 0.4f, 0.7f));
	for (const CircleData& circle : engine.intersect_circles2) {
		float size = 0.3;
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(circle.x, circle.y, 0.0f));
		model = glm::scale(model, glm::vec3(size, size, 1.0f));
		glm::mat4 mvp = getMVP(model);
		m_circle_shader.setMVP(mvp);
		Quad::draw();
	}

	//drawing test line
	Point p = engine.test_line.getStart();
	glm::vec3 point_scale = glm::vec3(0.2f, 0.2f, 1.0f);
	m_circle_shader.use();
	m_circle_shader.setVec3("colour", Colour::blue().toVec3());
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(p.x, p.y, 0.0f));
	model = glm::scale(model, point_scale);
	glm::mat4 mvp = getMVP(model);
	m_circle_shader.setMVP(mvp);
	Quad::draw();

	model = glm::mat4(1.0f);
	Point p2 = engine.test_line.getEnd();
	model = glm::translate(model, glm::vec3(p2.x, p2.y, 0.0f));
	model = glm::scale(model, point_scale);
	mvp = getMVP(model);
	m_circle_shader.setMVP(mvp);
	Quad::draw();

	Vector2D dir = engine.test_line.getDirection();
	LineDrawer::draw(p.x, p.y, p.x + dir.getX(), p.y + dir.getY(), m_mvp);

	//draw circle[1] goal
	const MapPosition* target = circles[1].getCurrentTarget();
	if (target != nullptr) {
		m_circle_shader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(target->x, target->y, 0.0f));
		glm::mat4 mvp = getMVP(model);
		m_circle_shader.setMVP(mvp);
		Quad::draw();
	}


	glDisable(GL_SCISSOR_TEST);

	//DrawText::renderText(std::to_string(width), 200, 0);
	//DrawText::renderText(std::to_string(getScaleFromMatrix(test_model)[0]), 400, 0);
}

void CircleTestEngineRenderer::renderGrid(CircleTestEngine& engine) {
	const RectBound& view_bound = engine.getViewBound();
	Colour line_colour = Colour(100, 200, 200);
	int lower_x = static_cast<int>(view_bound.left);
	if (view_bound.left > 0) {
		lower_x += 1;
	}
	int upper_x = static_cast<int>(view_bound.right);
	LineDrawer::setColour(line_colour);
	for (int x = lower_x; x <= upper_x; ++x) {
		LineDrawer::draw(x, view_bound.top, x, view_bound.bottom, m_mvp);
	}

	int lower_y = static_cast<int>(view_bound.top);
	if (view_bound.top > 0) {
		lower_y += 1;
	}
	int upper_y = static_cast<int>(view_bound.bottom);
	for (int y = lower_y; y <= upper_y; ++y) {
		LineDrawer::draw(view_bound.left, y, view_bound.right, y, m_mvp);
	}
}

const glm::mat4& CircleTestEngineRenderer::getMVP(const glm::mat4& model) {
	return m_mvp * model;
}
