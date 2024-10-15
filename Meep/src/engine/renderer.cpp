#include "renderer.hpp"


AppRenderer::AppRenderer(): 
m_canvas_projection(AppEngine::getFullProjection())
{
	setupShaders();
};

void AppRenderer::renderEngine(AppEngine& engine) {
	
	std::vector<InterfaceWindow> windows = engine.getWindows();

	m_colour_shader.use();
	glm::mat4 projection = AppEngine::getFullProjection();
	m_colour_shader.setProjection(projection);
	for (InterfaceWindow &iw : windows) {
		Colour c = iw.getBackgroundColour();
		m_colour_shader.setVec3("colour", c.toVec3());

		Dimensions dims = iw.getDimensions();
		Position pos = iw.getPosition();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(pos.x + dims.width / 2.0f, pos.y + dims.height / 2.0f, 0));
		model = glm::scale(model, glm::vec3(dims.width, dims.height, 1));
		m_colour_shader.setModel(model);
		Quad::draw();

	}
	if (windows.size() > 0) {
		//m_meep_renderer.renderTest(windows[0]);
		m_meep_renderer.renderMeepEngine(engine.getMeepEngine(), windows[0]);
	}

}

void AppRenderer::setupShaders() {
	Shader::FragmentShader full_colour = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(full_colour);
	m_colour_shader.use();
	glm::mat4 identity(1.0f);
	m_colour_shader.setModel(identity);
	m_colour_shader.setView(identity);
	m_colour_shader.setProjection(identity);
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

void MeepRenderer::setShaderViewProjection(const MeepEngine& engine, InterfaceWindow& window) {
	Position pos = window.getPosition();
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

void MeepRenderer::renderMeepEngine(const MeepEngine& engine, InterfaceWindow& window) {
	setShaderViewProjection(engine, window);

	Position pos = window.getPosition();
	Dimensions dims = window.getDimensions();

	std::map<unsigned int, Meep> meeps = engine.getMeeps();
	glScissor(pos.x, AppEngine::s_window_dimensions.height - dims.height - pos.y, dims.width, dims.height); // y - accounts for y starting at bottom
	glEnable(GL_SCISSOR_TEST);

	std::map<unsigned int, Food> foods = engine.getFoods();

	renderMeeps(meeps);
	renderFoods(foods);
	/*
	* draw full screen quad

	m_colour_shader.use();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(engine.getWidth() / 2, engine.getHeight() / 2, 0));
	model = glm::scale(model, glm::vec3(engine.getWidth(), engine.getHeight(), 1));
	m_colour_shader.setModel(model);
	Quad::draw();
	*/

	glDisable(GL_SCISSOR_TEST);
}

void MeepRenderer::renderMeeps(std::map<unsigned int, Meep>& meeps) {
	m_meep_shader.use();
	for (auto const& [id, meep] : meeps) {
		float scale = meep.getScale();
		float rotation = meep.getRotation();
		m_meep_shader.setFloat("rotation", rotation);
		//m_meep_shader.setVec2("direction", glm::vec2(meep.getDirection()));
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(meep.getX(), meep.getY(), 0.1));
		model = glm::scale(model, glm::vec3(scale, scale, 1));
		m_meep_shader.setModel(model);
		Quad::draw();
	}
}

void MeepRenderer::renderFoods(std::map<unsigned int, Food>& foods) {
	m_circle_shader.use();
	m_circle_shader.setVec3("colour", glm::vec3(Colour(100, 50, 50).toVec3()));
	for (auto const& [id, food] : foods) {
		float scale = food.getScale();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(food.getX(), food.getY(), 0.05));
		model = glm::scale(model, glm::vec3(scale, scale, 1));
		m_circle_shader.setModel(model);
		Quad::draw();
	}
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

	std::map<unsigned int, Meep> meeps = eng.getMeeps();
	std::optional<unsigned int> hovered_meep_id = eng.getHoveredMeepId();
	for (auto const& [id, meep] : meeps) {
		if (!(hovered_meep_id.has_value() && hovered_meep_id.value() == id)) {
			renderMeepNormal(meep);
		}
	}
	if (hovered_meep_id.has_value()) {
		renderMeepOutline(meeps.find(hovered_meep_id.value())->second);
	}

	m_colour_shader.use();
	m_colour_shader.setVec3("colour", colour);
	for (auto const& [id, food] : eng.getFoods()) {
		float size = food.getSize();
		QuadCircle circ;
		circ.setTranslateScale(food.getX(), food.getY(), size, size);
		circ.draw();
	}

	if (meeps.find(0) != meeps.end()) {
		//Meep& meep = meeps[0];
		//float energy = meep.getEnergy();
		//float max_energy = meep.getMaxEnergy();
		//float energy_ratio = meep.getEnergyRatio();

		//m_colour_shader.setMatrix4("projection", m_canvas_projection);
		//StatusBar sb(200.0f, 200.0f, 100.0f, 25.0f, energy_ratio);
		//sb.setStatusValue(energy_ratio);
		//sb.draw(m_colour_shader);

	}

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