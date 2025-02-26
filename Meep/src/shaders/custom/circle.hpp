#pragma once
//https://stackoverflow.com/questions/9351132/two-files-of-the-same-name-give-linker-error-in-visual-studio
#include "../library.hpp"
#include "../../utils/colour.hpp"

namespace CustomShader{

//can abstract circle shaders

class SolidCircleShader {
public:
	//static void setup();
	SolidCircleShader();
	void use();
	void setMVP(const glm::mat4& mvp);
	void setCircleColour(const Colour& colour);
	void setBackgroundColour(const Colour& colour);
	void setCircleCentre(float x, float y);
	void setRadius(float radius);
private:
	
	void setup();
	ShaderLibrary::MVPShader m_shader;

	static void fragmentShaderSetup();
	static std::optional<Shader::FragmentShader> s_fragment_shader;
};

class OutlineCircleShader {
public:
	OutlineCircleShader();
	void use();
	void setMVP(const glm::mat4& mvp);
	void setOutlineColour(const Colour& colour);
	void setCircleCentre(float x, float y);
	void setRadius(float radius);
	void setOutlineWidth(float width);
private:
	void setup();

	ShaderLibrary::MVPShader m_shader;

	static void fragmentShaderSetup();
	static std::optional<Shader::FragmentShader> s_fragment_shader;
};

}

