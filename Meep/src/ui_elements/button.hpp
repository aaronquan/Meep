#pragma once

#include <functional>
#include <string>

#include "../shaders/library.hpp"
#include "../shapes/shapes.hpp"
#include "../text/text.hpp"

class Button {
public:
	Button(float x, float y, float width, float height);
	void draw(ShaderLibrary::MVPShader& shader, const glm::mat4& vp) const;
	void draw(ShaderLibrary::MVPShader& shader, const glm::mat4& projection, const glm::mat4& view) const;

	bool isHovered() const;

	void setLabel(std::string label);
	void setLabelScale(float scale);
	void setTextColour(Colour colour);

	void setButtonPressed(std::function<void(void*, Button*)> f, void* args=nullptr);
	void setHoverIn(std::function<void(void*, Button*)> f, void* args=nullptr);
	void setHoverOut(std::function<void(void*, Button*)> f, void* args=nullptr);

	void mouseDownEvent();
	void mouseMoveEvent(float x, float y); // 

private:
	void drawLabel(const glm::mat4& vp) const;

	float m_x;
	float m_y;
	float m_width;
	float m_height;
	bool m_hovered;
	
	std::string m_label;
	float m_label_scale;
	Colour m_text_colour;

	std::function<void(void*, Button*)> m_on_pressed;
	void* m_on_pressed_args;

	std::function<void(void*, Button*)> m_on_hover_in;
	void* m_on_hover_in_args;

	std::function<void(void*, Button*)> m_on_hover_out;
	void* m_on_hover_out_args;
};