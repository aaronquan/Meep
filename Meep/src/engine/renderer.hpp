#pragma once
#include <glm/glm.hpp>
#include "engine.hpp"

//#include "shader/shader.hpp"

#include "../shaders/shader.hpp"
#include "../shaders/library.hpp"

#include "../shapes/shapes.hpp"

#include "../meep/meep.hpp"
#include "../meep/food.hpp"

#include "../text/text.hpp"

#include "../shapes/shapes.hpp"

class MeepRenderer {
public:
	MeepRenderer();
	void renderMeepEngine(const MeepEngine& engine, InterfaceWindow& window);

private:
	void setupShaders();
	void setCoordinateShader(ShaderLibrary::CoordinateShader& shader);
	void setShaderViewProjection(const MeepEngine& engine, InterfaceWindow& window);

	void renderMeeps(std::map<unsigned int, Meep>& meeps);
	void renderFoods(std::map<unsigned int, Food>& foods);

	ShaderLibrary::CoordinateShader m_circle_shader;
	ShaderLibrary::CoordinateShader m_meep_shader;
	ShaderLibrary::CoordinateShader m_colour_shader;
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

	void renderMeepNormal(const Meep &meep);

	void renderMeepOutline(const Meep& meep);
};

class AppRenderer {
public:
	AppRenderer();

	void renderEngine(AppEngine& engine);
private:
	void setupShaders();
	ShaderLibrary::CoordinateShader m_colour_shader;
	glm::mat4 m_canvas_projection;

	MeepRenderer m_meep_renderer;
};