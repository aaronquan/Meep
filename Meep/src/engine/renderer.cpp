#include "renderer.hpp"

#include "../math/matrix.hpp"

AppRenderer::AppRenderer(): 
m_canvas_projection(AppEngine::getFullProjection())
{
	setupShaders();
};

void AppRenderer::renderEngine(AppEngine& engine) {
	
	renderEngineWindows(engine);
	m_meep_renderer.renderMeepEngine(engine.getMeepEngine(), engine.getMeepWindow());

	m_circle_test_renderer.render(engine.getCircleTestEngine(), engine.getCircleTestWindow());

	const InterfaceWindow& information = engine.getInformationWindow();
	WindowPosition info_mouse = information.getPosition();
	WindowPosition* meep_mouse = engine.getMeepEngineMouse();
	std::string position_text;
	if (meep_mouse == nullptr) {
		position_text = "Outside";
	}
	else {
		std::stringstream position_stream;
		position_stream << std::fixed << std::setprecision(2) << meep_mouse->x << 
			", " << meep_mouse->y;
		position_text = position_stream.str();
	}
	DrawText::renderText(position_text, info_mouse.x, info_mouse.y, 1);

	float fps = engine.getFPS();
	std::string fps_text = std::format("{:.2f}", fps);

	DrawText::renderText(fps_text, 0, 0);

	std::optional<MapPosition> circle_test_mouse = engine.getCircleTestEngine().getMouse();
	std::string test_mouse_text;
	if (circle_test_mouse.has_value()) {
		MapPosition mouse = circle_test_mouse.value();
		test_mouse_text = std::format("{:.1f} {:.1f}", mouse.x, mouse.y);
	}
	else {
		test_mouse_text = "Not inside test";
	}
	DrawText::renderText(test_mouse_text, info_mouse.x, info_mouse.y+25, 1);
	DrawText::renderText("Window Size", 40, 320, 0.7);

	renderTextInputs(engine);
	
	glm::mat4 mvp;
	/*
	mvp = glm::ortho(0.0f, static_cast<float>(AppEngine::s_window_dimensions.width), 
		0.0f, static_cast<float>(AppEngine::s_window_dimensions.height));
	glm::mat4 test_model(1.0f);
	//test_model = glm::scale(test_model, glm::vec3(1.1f, 1.1f, 1.0f));
	test_model = glm::translate(test_model, glm::vec3(20.0f, 100.0f, 0.0f));
	//test_model = glm::scale(test_model, glm::vec3(1.1f, 1.1f, 1.0f));
	mvp *= test_model;

	DrawText::renderTextMVP("h", mvp);
	*/

	mvp = glm::ortho(0.0f, static_cast<float>(AppEngine::s_window_dimensions.width),
		static_cast<float>(AppEngine::s_window_dimensions.height), 0.0f);
	glm::mat4 test_model2(1.0f);
	glm::mat4 test_mvp = mvp;
	//test_model2 = glm::scale(test_model2, glm::vec3(5.1f, 5.1f, 1.0f));
	test_model2 = glm::translate(test_model2, glm::vec3(10.0f, 10.0f, 0.0f));
	test_model2 = glm::scale(test_model2, glm::vec3(1.0f, 1.0f, 1.0f));
	test_mvp *= test_model2;

	DrawText::renderTextMVP("yn", test_mvp);

	std::vector<Button> buttons = engine.getButtons();
	//m_colour_shader2.use();
	Colour normal_colour = Colour::fromInt(100, 100, 100);
	Colour hover_colour = Colour::fromInt(60, 60, 100);
	for (const Button& button : buttons) {
		if (button.isHovered()) {
			m_colour_shader2.use();
			m_colour_shader2.setVec3("colour", hover_colour.toVec3());
		}else{
			m_colour_shader2.use();
			m_colour_shader2.setVec3("colour", normal_colour.toVec3());
		}
		button.draw(m_colour_shader2, mvp);
	}
}

void AppRenderer::renderEngineWindows(AppEngine& engine) {
	m_colour_shader.use();
	glm::mat4 projection = AppEngine::getFullProjection();
	m_colour_shader.setProjection(projection);
	std::vector<InterfaceWindow> windows = engine.getWindows();
	for (const InterfaceWindow& iw : windows) {
		//renderWindow(iw);
	}

	renderWindow(engine.getMeepWindow());
	renderWindow(engine.getInformationWindow());
	renderWindow(engine.getCircleTestWindow());

	m_colour_shader2.use();
	m_colour_shader2.setVec3("colour", Colour::fromInt(120, 50, 60).toVec3());

	/*
	const std::vector<TextInput>& text_inputs = engine.getTextInputs();
	for (const TextInput& ti : text_inputs) {
		if (!ti.isHighlighted()) {
			m_colour_shader2.use();
			m_colour_shader2.setVec3("colour", Colour(120, 50, 140).toVec3());
			ti.draw(m_colour_shader2);
		}
		else {

		}
	}*/
	
	/*
	m_colour_shader2.use();
	m_colour_shader2.setVec3("colour", Colour(200, 50, 60).toVec3());
	TextInput* highlighted_input = TextInput::getHighlightedInput();
	if (highlighted_input != nullptr) {
		highlighted_input->draw(m_colour_shader2);
		m_colour_shader2.use();
		m_colour_shader2.setVec3("colour", Colour(0, 0, 0).toVec3());
		TextInput::drawHighlightedCursor(m_colour_shader2);
	}*/
}

void AppRenderer::renderTextInputs(AppEngine& engine){
	const std::vector<TextInput>& text_inputs = engine.getTextInputs();
	bool has_highlighted = false;
	for (const TextInput& ti : text_inputs) {
		m_colour_shader2.use();
		if (!ti.isHighlighted()) {
			m_colour_shader2.setVec3("colour", Colour::fromInt(120, 50, 140).toVec3());
		}
		else {
			m_colour_shader2.setVec3("colour", Colour::fromInt(230, 150, 140).toVec3());
			has_highlighted = true;
		}
		ti.draw(m_colour_shader2);
	}
	const std::vector<NumberInput>& number_inputs = engine.getNumberInputs();
	for (const TextInput& ti : number_inputs) {
		m_colour_shader2.use();
		if (!ti.isHighlighted()) {
			m_colour_shader2.setVec3("colour", Colour::fromInt(120, 50, 140).toVec3());
		}
		else {
			m_colour_shader2.setVec3("colour", Colour::fromInt(230, 150, 140).toVec3());
			has_highlighted = true;
		}
		ti.draw(m_colour_shader2);
	}
	if (has_highlighted) {
		m_colour_shader2.use();
		m_colour_shader2.setVec3("colour", Colour::fromInt(0, 0, 0).toVec3());
		TextInput::drawHighlightedCursor(m_colour_shader2);
	}
}

void AppRenderer::renderWindow(const InterfaceWindow& window) {
	/*
	m_colour_shader.use();
	Colour c = window.getBackgroundColour();
	m_colour_shader.setVec3("colour", c.toVec3());

	Dimensions dims = window.getDimensions();
	WindowPosition pos = window.getPosition();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(pos.x + dims.width / 2.0f, pos.y + dims.height / 2.0f, 0));
	model = glm::scale(model, glm::vec3(dims.width, dims.height, 1));
	m_colour_shader.setModel(model);
	Quad::draw();*/

	m_colour_shader2.use();
	Colour c = window.getBackgroundColour();
	m_colour_shader2.setVec3("colour", c.toVec3());

	Dimensions dims = window.getDimensions();
	WindowPosition pos = window.getPosition();
	glm::mat4 mvp(1.0f);
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(pos.x + dims.width / 2.0f, pos.y + dims.height / 2.0f, 0));
	model = glm::scale(model, glm::vec3(dims.width, dims.height, 1));
	glm::mat4 projection = AppEngine::getFullProjection();
	mvp = projection * model;
	m_colour_shader2.setMVP(mvp);
	
	//glEnable(GL_BLEND);
	
	/*
	m_solid_circle_shader.use();
	m_solid_circle_shader.setMVP(mvp);
	Colour c1 = Colour::clear();
	c1.setAlpha(0.5f);
	m_solid_circle_shader.setBackgroundColour(c1);
	m_solid_circle_shader.setCircleColour(Colour::red());
	m_solid_circle_shader.setRadius(0.5f);
	m_solid_circle_shader.setCircleCentre(0.5f, 0.5f);
	*/
	/*
	m_outline_circle_shader.use();
	m_outline_circle_shader.setMVP(mvp);
	m_outline_circle_shader.setOutlineWidth(0.05f);
	m_outline_circle_shader.setRadius(0.4f);
	*/
	Quad::draw();

	//glDisable(GL_BLEND);

	//LineCircleDrawer::draw(150.0f, 20.0f, 40.0f, projection);

}

void AppRenderer::setupShaders() {
	Shader::FragmentShader full_colour = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(full_colour);
	m_colour_shader.use();
	glm::mat4 identity(1.0f);
	m_colour_shader.setModel(identity);
	m_colour_shader.setView(identity);
	m_colour_shader.setProjection(identity);


	m_colour_shader2.addFragmentShader(full_colour);
	m_colour_shader2.setMVP(identity);


	//m_solid_circle_shader.setup();

}

MeepRenderer::MeepRenderer(){
	setupShaders();
};

void MeepRenderer::setupShaders() {
	Shader::FragmentShader circle = ShaderLibrary::generateFragmentShader("circle.frag");
	m_circle_shader.addFragmentShader(circle);
	setCoordinateShader(m_circle_shader);

	Shader::FragmentShader meep_shader = ShaderLibrary::generateFragmentShader("meep.frag");
	m_meep_shader.addFragmentShader(meep_shader);
	setCoordinateShader(m_meep_shader);

	Shader::FragmentShader color = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(color);
	setCoordinateShader(m_colour_shader);
}

void MeepRenderer::setCoordinateShader(ShaderLibrary::CoordinateShader& shader) {
	shader.use();
	glm::mat4 identity(1.0f);
	shader.setModel(identity);
	shader.setView(identity);
	shader.setProjection(identity);
}

void MeepRenderer::setShaderViewProjection(const MeepEngine& engine, const InterfaceWindow& window) {
	WindowPosition pos = window.getPosition();
	Dimensions dims = window.getDimensions();
	float zoom = 1.0f;
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(
		pos.x * (engine.getWidth() / AppEngine::s_window_dimensions.width),
		pos.y * (engine.getHeight() / AppEngine::s_window_dimensions.height),
		0));

	view = glm::scale(view, glm::vec3(
		zoom * (static_cast<float>(dims.width) / AppEngine::s_window_dimensions.width),
		zoom * (static_cast<float>(dims.height) / AppEngine::s_window_dimensions.height),
		1)
	);
	glm::mat4 proj = glm::ortho(
		0.0f,
		static_cast<float>(engine.getWidth()),
		static_cast<float>(engine.getHeight()),
		0.0f,
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::max()
	);
	m_circle_shader.use();
	m_circle_shader.setProjection(proj);
	m_circle_shader.setView(view);

	m_meep_shader.use();
	m_meep_shader.setProjection(proj);
	m_meep_shader.setView(view);

	m_colour_shader.use();
	m_colour_shader.setProjection(proj);
	m_colour_shader.setView(view);
}

void MeepRenderer::renderMeepEngine(const MeepEngine& engine, const InterfaceWindow& window) {
	setShaderViewProjection(engine, window);

	WindowPosition pos = window.getPosition();
	const Dimensions& dims = window.getDimensions();

	//std::map<unsigned int, Meep*> meeps = engine.getMeeps();
	int scissor_y = AppEngine::s_window_dimensions.height - dims.height - static_cast<int>(pos.y);
	glScissor(pos.x, scissor_y, dims.width, dims.height); // y - accounts for y starting at bottom
	glEnable(GL_SCISSOR_TEST);

	//std::map<unsigned int, Food*> foods = engine.getFoods();

	renderMeeps(engine);
	renderFoods(engine);
	
	//draw full screen quad

	m_colour_shader.use();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(engine.getWidth() / 2, engine.getHeight() / 2, 0));
	model = glm::scale(model, glm::vec3(engine.getWidth(), engine.getHeight(), 1));
	m_colour_shader.setModel(model);
	//glm::vec3 scale = getScaleFromMatrix(model);
	//std::cout << scale.x << '\n';
	Quad::draw(); 


	glDisable(GL_SCISSOR_TEST);
}


void MeepRenderer::renderMeeps(const MeepEngine& engine) {
	std::map<unsigned int, Meep*> meeps = engine.getMeeps();
	std::optional<unsigned int> hovered_meep_id = engine.getHoveredMeepId();
	if (hovered_meep_id.has_value()) {
		renderMeepOutline(*meeps[hovered_meep_id.value()]);
	}
	m_meep_shader.use();
	for (auto const& [id, meep] : meeps) {
		//if (!(hovered_meep_id.has_value() && hovered_meep_id == id)) {
		renderMeepNormal(*meep);
		//}
	}
}

void MeepRenderer::renderFoods(const MeepEngine& engine) {
	std::map<unsigned int, Food*> foods = engine.getFoods();
	m_circle_shader.use();
	m_circle_shader.setVec3("colour", glm::vec3(Colour::fromInt(100, 50, 50).toVec3()));
	for (auto const& [id, food] : foods) {
		float scale = food->getScale();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(food->getX(), food->getY(), 0.05));
		model = glm::scale(model, glm::vec3(scale, scale, 1));
		m_circle_shader.setModel(model);
		Quad::draw();
	}
}

void MeepRenderer::renderMeepNormal(const Meep& meep) {
	float scale = meep.getScale();
	float rotation = meep.getRotation();
	m_meep_shader.setFloat("rotation", rotation);
	//m_meep_shader.setVec2("direction", glm::vec2(meep.getDirection()));
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(meep.getX(), meep.getY(), -1));
	model = glm::scale(model, glm::vec3(scale, scale, 1));
	m_meep_shader.setModel(model);
	Quad::draw();
}

void MeepRenderer::renderMeepOutline(const Meep& meep) {
	
	m_circle_shader.use();
	m_circle_shader.setVec3("colour", glm::vec3(1, 1, 1));
	float scale = meep.getScale();
	scale += 1;
	float rotation = meep.getRotation();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(meep.getX(), meep.getY(), 0));
	model = glm::scale(model, glm::vec3(scale, scale, 1));
	m_circle_shader.setModel(model);
	Quad::draw();
	
	//m_meep_shader.use();
	//renderMeepNormal(meep);

}

Renderer2D::Renderer2D(unsigned int width, unsigned int height) :
	m_colour_shader(),
	m_canvas_projection(glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -100.0f, 100.0f)),
	m_engine_projection(glm::mat4(1.0f))
{};

Renderer2D::Renderer2D() : Renderer2D(100, 100) {};

void Renderer2D::setup(MeepEngine eng) {
	Shader::FragmentShader colour_block_shader = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(colour_block_shader);

	m_outline_shader.addFragmentShader(colour_block_shader);

	//Triangle::setup();
	//QuadCircle::setup();
	//Quad::setup();
	//StatusBar::setup();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(0.0f, eng.getWidth(), eng.getHeight(), 0.0f, -100.0f, 100.0f);
	m_engine_projection = projection;

	//std::cout << eng.getWidth() << ' ' << eng.getHeight() << std::endl;
	m_colour_shader.use();
	m_colour_shader.setMat4("model", model);
	m_colour_shader.setMat4("view", view);
	m_colour_shader.setMat4("projection", projection);


	m_outline_shader.use();
	m_outline_shader.setMat4("model", model);
	m_outline_shader.setMat4("view", view);
	m_outline_shader.setMat4("projection", projection);

	//StatusBar::setProjection(projection);
}

void Renderer2D::clean() {
	Triangle::clean();
	QuadCircle::clean();
	Quad::clean();
}

void Renderer2D::render(MeepEngine eng) {
	float meep_colour[] = { 0.4f, 0.3f, 0.9f };
	glm::vec3 colour(0.4f, 0.3f, 0.9f);

	float food_colour[] = { 0.7f, 0.5f, 0.2f };

	float health_colour[] = { 0.1f, 0.9f, 0.1f };
	float health_back_colour[] = { 0.9f, 0.1f, 0.1f };
	m_colour_shader.use();
	m_colour_shader.setMat4("projection", m_engine_projection);
	m_colour_shader.setVec3("colour", colour);
	/*
	std::map<unsigned int, Meep*> meeps = eng->getMeeps();
	std::optional<unsigned int> hovered_meep_id = eng.getHoveredMeepId();
	for (auto const& [id, meep] : meeps) {
		if (!(hovered_meep_id.has_value() && hovered_meep_id.value() == id)) {
			renderMeepNormal(meep);
		}
	}
	if (hovered_meep_id.has_value()) {
		renderMeepOutline(meeps.find(hovered_meep_id.value())->second);
	}*/

	m_colour_shader.use();
	m_colour_shader.setVec3("colour", colour);
	for (auto const& [id, food] : eng.getFoods()) {
		float size = food->getSize();
		QuadCircle circ;
		circ.setTranslateScale(food->getX(), food->getY(), size, size);
		circ.draw();
	}

	//if (meeps.find(0) != meeps.end()) {
		//Meep& meep = meeps[0];
		//float energy = meep.getEnergy();
		//float max_energy = meep.getMaxEnergy();
		//float energy_ratio = meep.getEnergyRatio();

		//m_colour_shader.setMatrix4("projection", m_canvas_projection);
		//StatusBar sb(200.0f, 200.0f, 100.0f, 25.0f, energy_ratio);
		//sb.setStatusValue(energy_ratio);
		//sb.draw(m_colour_shader);

	//}

};

void Renderer2D::renderMeepNormal(const Meep& meep) {
	float size = meep.getSize();
	QuadCircle circ;
	circ.setTranslateScale(meep.getX(), meep.getY(), size, size);
	circ.draw();
}

void Renderer2D::renderMeepOutline(const Meep& meep) {
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	float size = meep.getSize();
	QuadCircle circ;
	circ.setTranslateScale(meep.getX(), meep.getY(), size, size);
	circ.draw();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);

	float scale = size * 1.05f;
	m_outline_shader.use();
	circ.setTranslateScale(meep.getX(), meep.getY(), scale, scale);
	circ.draw();
	glBindVertexArray(0);
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
}