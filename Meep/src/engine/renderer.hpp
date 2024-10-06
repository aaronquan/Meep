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

class AppRenderer {
public:
	AppRenderer();

	void renderEngine(AppEngine engine);
private:
	void setupShaders();
	ShaderLibrary::CoordinateShader m_colour_shader;
};

class Renderer2D {
public:
	Renderer2D();
	Renderer2D(unsigned int width, unsigned int height);

	void setup(Engine eng);

	void clean();

	void render(Engine eng);

private:
	ShaderLibrary::TransformShader m_colour_shader;
	ShaderLibrary::TransformShader m_outline_shader;

	glm::mat4 m_engine_projection;
	glm::mat4 m_canvas_projection;

	void renderMeepNormal(const Meep &meep);

	void renderMeepOutline(const Meep& meep);
};