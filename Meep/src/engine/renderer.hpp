#pragma once

#include <iomanip>
#include <format>

#include <glm/glm.hpp>
#include "engine.hpp"

//#include "shader/shader.hpp"

#include "../shaders/shader.hpp"
#include "../shaders/library.hpp"
#include "../shaders/custom.hpp"

#include "../shapes/shapes.hpp"

#include "../meep/meep.hpp"
#include "../meep/food.hpp"

#include "../text/text.hpp"
#include "../text/text_input.hpp"

#include "../shapes/shapes.hpp"

#include "test_renderer.hpp"

class MeepRenderer {
public:
	MeepRenderer();
	void renderMeepEngine(const MeepEngine& engine, const InterfaceWindow& window);

private:
	void setupShaders();
	void setCoordinateShader(ShaderLibrary::CoordinateShader& shader);
	void setShaderViewProjection(const MeepEngine& engine, const InterfaceWindow& window);

	void renderMeeps(const MeepEngine& engine);
	void renderFoods(const MeepEngine& engine);


	void renderMeepNormal(const Meep& meep);
	void renderMeepOutline(const Meep& meep);

	ShaderLibrary::CoordinateShader m_circle_shader;
	ShaderLibrary::CoordinateShader m_meep_shader;
	//ShaderLibrary::CoordinateShader m_meep_outline_shader;
	ShaderLibrary::CoordinateShader m_colour_shader;
};

class AppRenderer {
public:
	AppRenderer();

	void renderEngine(AppEngine& engine);
private:
	void setupShaders();
	void renderEngineWindows(AppEngine& engine);
	void renderTextInputs(AppEngine& engine);
	void renderWindow(const InterfaceWindow& window);
	ShaderLibrary::CoordinateShader m_colour_shader;
	ShaderLibrary::MVPShader m_colour_shader2;
	glm::mat4 m_canvas_projection;


	MeepRenderer m_meep_renderer;
	CircleTestEngineRenderer m_circle_test_renderer;

	CustomShader::SolidCircleShader m_solid_circle_shader;
	CustomShader::OutlineCircleShader m_outline_circle_shader;
};

class Renderer2D {
public:
	Renderer2D();
	Renderer2D(unsigned int width, unsigned int height);

	void setup(MeepEngine eng);

	void clean();

	void render(MeepEngine eng);

private:
	ShaderLibrary::TransformShader m_colour_shader;
	ShaderLibrary::TransformShader m_outline_shader;

	glm::mat4 m_engine_projection;
	glm::mat4 m_canvas_projection;

	void renderMeepNormal(const Meep& meep);

	void renderMeepOutline(const Meep& meep);
};
