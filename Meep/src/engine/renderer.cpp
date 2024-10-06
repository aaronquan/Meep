#include "renderer.hpp"

AppRenderer::AppRenderer(){};

void AppRenderer::renderEngine(AppEngine engine) {
	//m_colour_shader.
	for (InterfaceWindow iw : engine.getWindows()) {
		
	}
}

void AppRenderer::setupShaders() {
	Shader::FragmentShader full_colour = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(full_colour);
	m_colour_shader.use();
	glm::mat4 identity(1.0);
	m_colour_shader.setProjection();
}

Renderer2D::Renderer2D(unsigned int width, unsigned int height) :
	m_colour_shader(),
	m_canvas_projection(glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -100.0f, 100.0f)),
	m_engine_projection(glm::mat4(1.0f))
{};

Renderer2D::Renderer2D() : Renderer2D(100, 100) {};

void Renderer2D::setup(Engine eng) {
	Shader::FragmentShader colour_block_shader = ShaderLibrary::generateFragmentShader("colour.frag");
	m_colour_shader.addFragmentShader(colour_block_shader);

	m_outline_shader.addFragmentShader(colour_block_shader);

	Triangle::setup();
	QuadCircle::setup();
	Quad::setup();
	//StatusBar::setup();

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(0.0f, eng.getWidth(), eng.getHeight(), 0.0f, -100.0f, 100.0f);
	m_engine_projection = projection;

	std::cout << eng.getWidth() << ' ' << eng.getHeight() << std::endl;
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

void Renderer2D::render(Engine eng) {
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
		Meep& meep = meeps[0];
		//float energy = meep.getEnergy();
		//float max_energy = meep.getMaxEnergy();
		float energy_ratio = meep.getEnergyRatio();

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