#include "circle.hpp"

namespace CustomShader {

//bool SolidCircleShader::s_is_fragment_setup = false;
std::optional<Shader::FragmentShader> SolidCircleShader::s_fragment_shader = std::nullopt;

SolidCircleShader::SolidCircleShader() 
	//: 
	//m_is_setup(false) 
{
	setup();
}

void SolidCircleShader::use() {
	m_shader.use();
}

void SolidCircleShader::setMVP(const glm::mat4& mvp) {
	m_shader.setMat4("mvp", mvp);
}

void SolidCircleShader::setCircleColour(const Colour& colour) {
	m_shader.setVec4("circle_colour", colour.toVec4());
}
void SolidCircleShader::setBackgroundColour(const Colour& colour) {
	m_shader.setVec4("background_colour", colour.toVec4());
}

void SolidCircleShader::setCircleCentre(float x, float y) {
	m_shader.setVec2("circle_centre", glm::vec2(x, y));
}

void SolidCircleShader::setRadius(float radius) {
	m_shader.setFloat("radius", radius);
}

void SolidCircleShader::fragmentShaderSetup() {
	if (!s_fragment_shader.has_value()) {
		s_fragment_shader = ShaderLibrary::generateFragmentShader("circle/solid_circle_tile.frag");
	}
}

void SolidCircleShader::setup() {
	//if (!m_is_setup) {
	m_shader.setup();
	fragmentShaderSetup();
	m_shader.addFragmentShader(s_fragment_shader.value());
	m_shader.use();
	//std::cout << "setup shader" << '\n';
	setMVP(glm::mat4(1.0f));
	setCircleColour(Colour::white());
	setBackgroundColour(Colour::red());
	setRadius(0.5f);
	setCircleCentre(0.5f, 0.5f);
	//}
}

std::optional<Shader::FragmentShader> OutlineCircleShader::s_fragment_shader = std::nullopt;

OutlineCircleShader::OutlineCircleShader() {
	setup();
};

void OutlineCircleShader::use() {
	m_shader.use();
}

void OutlineCircleShader::setMVP(const glm::mat4& mvp) {
	m_shader.setMat4("mvp", mvp);
}

void OutlineCircleShader::setOutlineColour(const Colour& colour) {
	m_shader.setVec4("outline_colour", colour.toVec4());
}

void OutlineCircleShader::setCircleCentre(float x, float y) {
	m_shader.setVec2("circle_centre", glm::vec2(x, y));
}

void OutlineCircleShader::setRadius(float radius) {
	m_shader.setFloat("radius", radius);
}

void OutlineCircleShader::setOutlineWidth(float width) {
	m_shader.setFloat("outline_width", width);
}

void OutlineCircleShader::fragmentShaderSetup() {
	if (!s_fragment_shader.has_value()) {
		s_fragment_shader = ShaderLibrary::generateFragmentShader("circle/outline_circle.frag");
	}
}

void OutlineCircleShader::setup() {
	m_shader.setup();
	fragmentShaderSetup();
	m_shader.addFragmentShader(s_fragment_shader.value());
	m_shader.use();
	setMVP(glm::mat4(1.0f));
	setOutlineColour(Colour::white());
	setRadius(0.5f);
	setCircleCentre(0.5f, 0.5f);
	setOutlineWidth(0.1f);
}


}