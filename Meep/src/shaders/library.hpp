#pragma once

//#include "shader_engine.hpp"
#include "shader.hpp"

#include <optional>
#include <iostream>

namespace ShaderLibrary{

Shader::VertexShader generateVertexShader(std::string vertex_source_file);

Shader::FragmentShader generateFragmentShader(std::string frag_source_file);

/*
class VertexTransformationShader : public Shader::VertexShader  {
public:
	VertexTransformationShader(): m_shader(generateVertexShader("transformation.vs")) {};
	Shader::VertexShader getShader() const {
		return m_shader;
	}
	void setTransformation(glm::vec4 transformation) {
		//m_shader.
	}
private:
	Shader::VertexShader m_shader;
};*/

class LibraryShader {
public:
	LibraryShader();
	bool isLinked() const;
	virtual std::string getShaderName() const = 0;
	virtual void setup() = 0;
	void use();
	bool addVertexShader(Shader::VertexShader& fs);
	bool addFragmentShader(Shader::FragmentShader& fs);

	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec2(const char* name, glm::vec2 v) const;
	void setVec3(const char* name, glm::vec3 v) const;
	void setVec4(const char* name, glm::vec4 v) const;
	void setMat4(const char* name, glm::mat4 v) const;
protected:
	Shader::MainShader m_shader;
	bool m_is_setup;
	//static std::string shader_name;
	//bool m_is_linked;
};

class FullScreenShader : public LibraryShader{
public:
	FullScreenShader();
	std::string getShaderName() const;
	void setup();
	//bool addFragment(Shader::FragmentShader& fs);
private:
	static std::optional<Shader::VertexShader> m_full_shader;
	//static std::string shader_name;
};

class ResolutionShader2D : public FullScreenShader {
public:
	//ResolutionShader2D();
	void setResolution(float width, float height);
	void setTime(float time);
private:
	
};

class GridShader2D : public ResolutionShader2D {
public:
	void setGridSize(int width, int height);
	void setTexture(unsigned int texture_id);
};

class TransformShader : public LibraryShader {
public:
	TransformShader();
	std::string getShaderName() const;
	void setup();
	void setTransform(glm::mat4 mat);
private:
	static std::optional<Shader::VertexShader> m_transform_shader;
};

class CoordinateShader : public LibraryShader {
public:
	CoordinateShader();
	std::string getShaderName() const;
	void setup();
	void setModel(glm::mat4& mat);
	void setView(glm::mat4& mat);
	void setProjection(glm::mat4& mat);
private:
	static std::optional<Shader::VertexShader> m_coordinate_shader;
};

/*

inline ShaderEngine::ShaderEngine generateDebugShaderEngine() {
	//vertex shaders
	Shader::VertexShader full_screen = generateVertexShader("full_screen.vs");
	Shader::VertexShader transformation = generateVertexShader("transformation.vs");
	Shader::VertexShader coordinate_system = generateVertexShader("coordinate_system.vs");

	//fragment shaders
	Shader::FragmentShader white = generateFragmentShader("white.fs");
	Shader::FragmentShader time = generateFragmentShader("time.fs");
	Shader::FragmentShader green_red = generateFragmentShader("green_red_gradient.fs");
	Shader::FragmentShader line = generateFragmentShader("line.fs");

	Shader::FragmentShader rect = generateFragmentShader("rectangle.fs");

	time.addVariable("u_time", Shader::Float);

	//shaders
	Shader::DebugShader white_screen;
	white_screen.addVertex(full_screen);
	white_screen.addFragment(white);
	white_screen.link();

	Shader::DebugShader time_screen;
	time_screen.addVertex(full_screen);
	time_screen.addFragment(time);
	time_screen.link();

	Shader::DebugShader green_red_gradient;
	green_red_gradient.addVertex(full_screen);
	green_red_gradient.addFragment(green_red);
	green_red_gradient.link();

	Shader::DebugShader full;
	full.addVertex(full_screen);
	full.link();

	Shader::DebugShader line_shader;
	line_shader.addFragment(line);
	line_shader.addVertex(full_screen);
	//line_shader.addFragment(line);
	line_shader.link();

	Shader::DebugShader rectangle;
	rectangle.addVertex(full_screen);
	rectangle.addFragment(rect);
	rectangle.link();

	Shader::DebugShader transform_white;
	transform_white.addVertex(transformation);
	transform_white.addFragment(white);
	transform_white.link();

	//add to engine
	ShaderEngine::ShaderEngine engine;
	engine.addDebugShader("white_screen", white_screen);
	engine.addDebugShader("time_screen", time_screen);

	engine.addDebugShader("green_red", green_red_gradient);
	engine.addDebugShader("full", full);
	engine.addDebugShader("line", line_shader);
	engine.addDebugShader("rectangle", rectangle);

	engine.addDebugShader("transform_white", transform_white);
	return engine;
}

*/

}