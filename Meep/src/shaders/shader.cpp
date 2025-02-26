#include "shader.hpp"

std::optional<std::string> textFileContents(const char* path);

namespace Shader {

	MainShader::MainShader(): 
	m_id(glCreateProgram()), 
	m_is_linked(false), 
	m_has_fragment(false),
	m_has_vertex(false)
	{};

	bool MainShader::isLinked() const {
		return m_is_linked;
	}
	bool MainShader::hasVertex() const {
		return m_has_vertex;
	}
	bool MainShader::hasFragment() const {
		return m_has_fragment;
	}

	void MainShader::addVertex(VertexShader& vs) {
		if(!m_has_vertex){
			glAttachShader(m_id, vs.getId());
			m_has_vertex = true;
		}
	}

	void MainShader::addFragment(FragmentShader& fs) {
		if(!m_has_fragment){	
			glAttachShader(m_id, fs.getId());
			m_has_fragment = true;
		}
	}

	void MainShader::use() {
		glUseProgram(m_id);
	}

	void MainShader::link() {
		if (!m_is_linked) {
			int success;
			char infoLog[512];
			glLinkProgram(m_id);
			// print linking errors if any
			glGetProgramiv(m_id, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_id, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				return;
			}
			m_is_linked = true;
		}
	}

	void MainShader::setInt(const char* name, int value) const {
		glUniform1i(getUniform(name), value);
	}

	void MainShader::setFloat(const char* name, float value) const {
		glUniform1f(getUniform(name), value);
		//return setParamFound(name);
	}

	void MainShader::setVec2(const char* name, glm::vec2 v) const {
		glUniform2f(getUniform(name), v.x, v.y);
		//return setParamFound(name);
	}

	void MainShader::setVec3(const char* name, glm::vec3 v) const {
		glUniform3f(getUniform(name), v.x, v.y, v.z);
	}

	void MainShader::setVec4(const char* name, glm::vec4 v) const {
		glUniform4f(getUniform(name), v.x, v.y, v.z, v.w);
	}

	void MainShader::setMat4(const char* name, const glm::mat4& m) const {
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &m[0][0]);
	}

	DebugShader::DebugShader() : m_id(glCreateProgram()), m_is_linked(false), m_first_used(false) {}
	void DebugShader::addVertex(VertexShader& vs) {
		glAttachShader(m_id, vs.getId());
		for (const auto& [name, type] : vs.getParameters()) {
			m_parameters.emplace(name, ShaderParameter(type));
			//std::cout << name << std::endl;
		}
	}
	void DebugShader::addFragment(FragmentShader& fs) {
		glAttachShader(m_id, fs.getId());
		for (const auto& [name, type] : fs.getParameters()) {
			m_parameters.emplace(name, ShaderParameter(type));
			//std::cout << name << std::endl;
		}
	}
	void DebugShader::use() {
		glUseProgram(m_id);
	}
	void DebugShader::useDebug(const char* shader_name, std::source_location source) {
		glUseProgram(m_id);
		if(!m_first_used){
			std::cout << "useDebug for " << shader_name << " called at " << 
			source.file_name() << " line " << source.line() << std::endl;
			logParamWarnings(shader_name);
			m_first_used = true;
		}
	}

	void DebugShader::logParamWarnings(const char* shader_name) const {
		for (const auto& [name, param] : m_parameters) {
			if (!param.is_set) {
				std::cout << "Warning: " << name << " is not set in shader - " << shader_name << param.is_set << std::endl;
			}
		}
	}

	bool DebugShader::link() {
		int success;
		char infoLog[512];
		glLinkProgram(m_id);
		// print linking errors if any
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success){
			glGetProgramInfoLog(m_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		m_is_linked = true;
		return success;
	}

	bool DebugShader::getIsLinked() const {
		return m_is_linked;
	}

	bool DebugShader::setFloat(const char* name, float value) {
		glUniform1f(getUniform(name), value);
		return setParamFound(name);
	}

	bool DebugShader::setVec2(const char* name, glm::vec2& v) {
		glUniform2f(getUniform(name), v.x, v.y);
		return setParamFound(name);
	}

	bool DebugShader::setVec2(const char* name, float v1, float v2) {
		glUniform2f(getUniform(name), v1, v2);
		return setParamFound(name);
	}

	bool DebugShader::setMat4(const char* name, const glm::mat4& mat) {
		glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, &mat[0][0]);
		return setParamFound(name);
	}

	bool DebugShader::setParamFound(const char* name){
		bool parameter_found = m_parameters.find(name) != m_parameters.end();
		if (parameter_found) {
			m_parameters[name].is_set = true;
		}
		return parameter_found;
	}

	ShaderComposition::ShaderComposition(unsigned int id) : m_id(id) {};

	VertexShader::VertexShader() : ShaderComposition(glCreateShader(GL_VERTEX_SHADER)) {};

	bool VertexShader::addSource(const char* source_code) {
		int success;
		char infoLog[512];

		glShaderSource(m_id, 1, &source_code, NULL);
		glCompileShader(m_id);
		// print compile errors if any
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(m_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		return success;
	}

	FragmentShader::FragmentShader() : ShaderComposition(glCreateShader(GL_FRAGMENT_SHADER)){};

	bool FragmentShader::addSource(const char* source_code) {
		int success;
		char infoLog[512];

		glShaderSource(m_id, 1, &source_code, NULL);
		glCompileShader(m_id);
		// print compile errors if any
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(m_id, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};
		return success;
	}


	std::optional<VertexShader> createVertexShaderFromFile(const char* path) {
		std::optional<std::string> contents = textFileContents(path);
		if (!contents.has_value()) {
			return std::nullopt;
		}
		const char* shaderCode = contents.value().c_str();
		VertexShader vs;
		bool success = vs.addSource(shaderCode);
		if (!success) {
			return std::nullopt;
		}
		return vs;
	}

	std::optional<FragmentShader> createFragmentShaderFromFile(const char* path) {
		std::optional<std::string> contents = textFileContents(path);
		if (!contents.has_value()) {
			return std::nullopt;
		}
		const char* shaderCode = contents.value().c_str();
		FragmentShader fs;
		bool success = fs.addSource(shaderCode);
		if (!success) {
			return std::nullopt;
		}
		return fs;
	}
}

//from fs/file.hpp
std::optional<std::string> textFileContents(const char* path) {
	std::string text;
	std::ifstream file;
	try {
		file.open(path);
		if (file.fail()) {
			std::cout << "No File Found: " << path << std::endl; 
			return std::nullopt;
		}
		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		text = stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER_ENGINE::FILE_NOT_SUCCESFULLY_READ " << path << std::endl;
		return std::nullopt;
	}
	return text;
}