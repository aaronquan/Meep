#include "library.hpp"

namespace ShaderLibrary {

Shader::VertexShader generateVertexShader(std::string vertex_source_file) {
	std::string path = "src/shaders/vertex/" + vertex_source_file;
	std::optional<Shader::VertexShader> opt_fs = Shader::createVertexShaderFromFile(path.c_str());
	if (!opt_fs.has_value()) {
		std::cout << "VertexShader failed compilation: " << vertex_source_file << std::endl;
		return Shader::VertexShader();
	}
	return opt_fs.value();
}

Shader::FragmentShader generateFragmentShader(std::string frag_source_file) {
	std::string path = "src/shaders/fragment/" + frag_source_file;
	std::optional<Shader::FragmentShader> opt_fs = Shader::createFragmentShaderFromFile(path.c_str());
	if (!opt_fs.has_value()) {
		std::cout << "FragmentShader failed compilation: " << frag_source_file << std::endl;
		return Shader::FragmentShader();
	}
	return opt_fs.value();
}

LibraryShader::LibraryShader(): m_shader(), m_is_setup(false){};

bool LibraryShader::isLinked() const {
	return m_shader.isLinked();
}

void LibraryShader::use() {
	if(!m_is_setup){
		setup();
		m_is_setup = true;
	}
	m_shader.link();
	if(isLinked()){
		m_shader.use();
	}
	else {
		std::cout << "Error: " << getShaderName() << " not linked" << std::endl;
	}
}

bool LibraryShader::addVertexShader(Shader::VertexShader& vs) {
	if (m_shader.hasVertex()) {
		std::cout << "Error: cannot add vertex shader to " << getShaderName() << ", already has vertex" << std::endl;
		return false;
	}
	if (isLinked()) {
		std::cout << "Error: cannot add vertex shader to " << getShaderName() << ", already linked" << std::endl;
		return false;
	}
	m_shader.addVertex(vs);
	return true;
}

bool LibraryShader::addFragmentShader(Shader::FragmentShader& fs) {
	if (m_shader.hasFragment()) {
		std::cout << "Error: cannot add fragment shader to " << getShaderName() << ", already has fragment" << std::endl;
		return false;
	}
	if (isLinked()) {
		std::cout << "Error: cannot add fragment shader to " << getShaderName() << ", already linked" << std::endl;
		return false;
	}

	m_shader.addFragment(fs);
	return true;
}

void LibraryShader::setInt(const char* name, int value) const {
	m_shader.setInt(name, value);
}

void LibraryShader::setFloat(const char* name, float value) const {
	m_shader.setFloat(name, value);
}

void LibraryShader::setVec2(const char* name, glm::vec2 v) const {
	m_shader.setVec2(name, v);
}

void LibraryShader::setVec3(const char* name, glm::vec3 v) const {
	m_shader.setVec3(name, v);
}

void LibraryShader::setVec4(const char* name, glm::vec4 v) const {
	m_shader.setVec4(name, v);
}

void LibraryShader::setMat4(const char* name, glm::mat4 m) const {
	m_shader.setMat4(name, m);
}

std::optional<Shader::VertexShader> FullScreenShader::m_full_shader = std::nullopt;

FullScreenShader::FullScreenShader() : LibraryShader(){};

std::string FullScreenShader::getShaderName() const {
	return "Full Screen Shader";
}

void FullScreenShader::setup() {
	if (!m_full_shader.has_value()) {
		m_full_shader = generateVertexShader("full_screen.vert");
	}
	m_shader.addVertex(m_full_shader.value());
}

void ResolutionShader2D::setResolution(float width, float height) {
	m_shader.setVec2("u_resolution", glm::vec2(width, height));
}

void ResolutionShader2D::setTime(float time) {
	m_shader.setFloat("u_time", time);
}

void GridShader2D::setGridSize(int width, int height) {
	m_shader.setVec2("grid_size", glm::vec2(width, height));
}

void GridShader2D::setTexture(unsigned int texture_id) {
	m_shader.setInt("ourTexture", texture_id);
}

std::optional<Shader::VertexShader> TransformShader::m_transform_shader = std::nullopt;

TransformShader::TransformShader(): LibraryShader(){};

std::string TransformShader::getShaderName() const {
	return "Transform Shader";
}

void TransformShader::setup() {
	if (!m_transform_shader.has_value()) {
		m_transform_shader = generateVertexShader("transformation.vert");
	}
	m_shader.addVertex(m_transform_shader.value());
}

void TransformShader::setTransform(glm::mat4 mat) {
	m_shader.setMat4("transform", mat);
}

std::optional<Shader::VertexShader> CoordinateShader::m_coordinate_shader = std::nullopt;

CoordinateShader::CoordinateShader() : LibraryShader() {};

std::string CoordinateShader::getShaderName() const {
	return "Coordinate Shader";
}

void CoordinateShader::setup() {
	if (!m_coordinate_shader.has_value()) {
		m_coordinate_shader = generateVertexShader("coordinate_2d.vert");
	}
	m_shader.addVertex(m_coordinate_shader.value());
}

void CoordinateShader::setModel(glm::mat4& mat) {
	m_shader.setMat4("model", mat);
}

void CoordinateShader::setView(glm::mat4& mat) {
	m_shader.setMat4("view", mat);
}

void CoordinateShader::setProjection(glm::mat4& mat) {
	m_shader.setMat4("projection", mat);
}

/*
TransformationShader2D::TransformationShader2D(): DebugShader(), m_transformation(1.0f){
	//if (m_transformation_shader.has_value()) {
		//addVertex(m_transformation_shader.value());
	//}else {
	//	std::cout << "Transformation not loaded" << std::endl;
	//}
};

void TransformationShader2D::translate(float x, float y) {
	m_transformation = glm::translate(m_transformation, glm::vec3(x, y, 0.0f));
	Shader::DebugShader::setMat4("transform", m_transformation);
}

void TransformationShader2D::rotate(float angle) {
	m_transformation = glm::rotate(m_transformation, angle, glm::vec3(0.0, 0.0, 1.0));
	Shader::DebugShader::setMat4("transform", m_transformation);
}

void TransformationShader2D::scale(float x, float y) {
	m_transformation = glm::scale(m_transformation, glm::vec3(x, y, 1.0f));
	Shader::DebugShader::setMat4("transform", m_transformation);
}
//void TransformationShader2D::set

void TransformationShader2D::setTransformation(glm::mat4& mat) {
	m_transformation = mat;
	setMat4("transform", mat);
}

void TransformationShader2D::use() {
	Shader::DebugShader::use();
	setMat4("transform", m_transformation);
}

void TransformationShader2D::setup() {
	if(!m_transformation_shader.has_value()){
		m_transformation_shader = generateVertexShader("transformation.vs");
	}
	addVertex(m_transformation_shader.value());
	link();
}

std::optional<Shader::VertexShader> TransformationShader2D::m_transformation_shader = std::nullopt;
*/

/*
CoordinateShader::CoordinateShader() : DebugShader(){};

void CoordinateShader::setModel(glm::mat4& mat) {
	setMat4("model", mat);
}

void CoordinateShader::setView(glm::mat4& mat) {
	setMat4("view", mat);
}

void CoordinateShader::setProjection(glm::mat4& mat) {
	setMat4("projection", mat);
}

void CoordinateShader::setup() {
	if (!m_coordinate_shader.has_value()) {
		m_coordinate_shader = generateVertexShader("coordinate.vs");
	}
	addVertex(m_coordinate_shader.value());
	link();
}

std::optional<Shader::VertexShader> CoordinateShader::m_coordinate_shader = std::nullopt;
*/

/*

WhiteTransformationShader::WhiteTransformationShader() : TransformationShader2D(){};

void WhiteTransformationShader::setup() {
	TransformationShader2D::setup();
	if (!m_transformation_shader.has_value()) {
		m_transformation_shader = generateVertexShader("transformation.vs");
	}
	addVertex(m_transformation_shader.value());
	if(!m_white_shader.has_value()){
		m_white_shader = generateFragmentShader("white.fs");
	}
	addFragment(m_white_shader.value());
	link();
	
}

std::optional<Shader::FragmentShader> WhiteTransformationShader::m_white_shader = std::nullopt;

PhongLightingShader::PhongLightingShader(): NormalCoordinateShader(){};

std::optional<Shader::FragmentShader> PhongLightingShader::m_phong_shader = std::nullopt;
*/
}
