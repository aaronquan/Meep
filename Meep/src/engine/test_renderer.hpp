#pragma once

#include "test.hpp"
#include "engine.hpp"
#include "../window/interface_window.hpp"

#include "../shaders/shader.hpp"
#include "../shaders/library.hpp"


class CircleTestEngineRenderer {
public:
	CircleTestEngineRenderer();
	void render(CircleTestEngine& engine, const InterfaceWindow& window);

private:
	void setupShaders();
	void setShaderViewProjection(CircleTestEngine& engine, const InterfaceWindow& window);

	void renderGrid(CircleTestEngine& engine);

	const glm::mat4& getMVP(const glm::mat4& model);
	ShaderLibrary::CoordinateShader m_circle_shader_old;
	ShaderLibrary::MVPShader m_circle_shader;
	ShaderLibrary::MVPShader m_colour_shader;

	//glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_mvp;
	//bool m_view_change;
};