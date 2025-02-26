#include "button.hpp"

//fix hover bug

Button::Button(float x, float y, float width, float height) :
	m_x(x), m_y(y),
	m_width(width), m_height(height),
	m_hovered(false),
	m_label(), m_label_scale(1.0f),
	m_on_pressed_args(nullptr),
	m_on_hover_in_args(nullptr),
	m_on_hover_out_args(nullptr)
{};

void Button::draw(ShaderLibrary::MVPShader& shader, const glm::mat4& vp) const {
	glm::mat4 mvp(1.0f);
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(m_x + (m_width / 2.0f), m_y + (m_height / 2.0f), 0.0f));
	model = glm::scale(model, glm::vec3(m_width, m_height, 1.0f));
	mvp = vp * model;
	shader.use();
	shader.setMVP(mvp);

	Quad::draw();

	drawLabel(vp);
}

void Button::draw(ShaderLibrary::MVPShader& shader, const glm::mat4& projection, const glm::mat4& view) const {
	glm::mat4 vp = projection * view;
	draw(shader, vp);
}

void Button::drawLabel(const glm::mat4& vp) const{
	float text_width = DrawText::textWidth(m_label, m_label_scale);
	float text_height = DrawText::textHeight(1, m_label_scale);
	//DrawText::renderText(std::to_string(m_label_scale), 400, 0);
	//DrawText::renderText(std::to_string(text_width), 200, 0);
	glm::mat4 mvp(1.0f);
	glm::mat4 model(1.0f);
	float translate_x = m_x + ((m_width - text_width) / 2.0f);
	float translate_y = m_y + ((m_height - text_height) / 2.0f);
	model = glm::translate(model, glm::vec3(translate_x, translate_y, 0.0f));
	model = glm::scale(model, glm::vec3(m_label_scale, m_label_scale, 1.0f));
	mvp = vp * model;

	DrawText::renderTextMVP(m_label, mvp, m_text_colour);
}

bool Button::isHovered() const {
	return m_hovered;
}

void Button::setLabel(std::string label) {
	m_label = label;
}

void Button::setLabelScale(float scale) {
	m_label_scale = scale;
}

void Button::setTextColour(Colour colour) {
	m_text_colour = colour;
}

void Button::setButtonPressed(std::function<void(void*, Button*)> f, void* args) {
	m_on_pressed = f;
	m_on_pressed_args = args;
}

void Button::setHoverIn(std::function<void(void*, Button*)> f, void* args) {
	m_on_hover_in = f;
	m_on_hover_in_args = args;
}

void Button::setHoverOut(std::function<void(void*, Button*)> f, void* args) {
	m_on_hover_out = f;
	m_on_hover_out_args = args;
}

void Button::mouseMoveEvent(float x, float y) {
	std::cout << x << ' ' << y << '\n';
	CollisionRect collision_quad(m_x, m_y, m_width, m_height);
	bool is_collision = collision_quad.isCollisionPoint(x, y);
	if (m_hovered != is_collision) {
		if (is_collision && m_on_hover_in) {
			m_on_hover_in(m_on_hover_in_args, this);
		}
		else if (!is_collision && m_on_hover_out) {
			m_on_hover_out(m_on_hover_out_args, this);
		}
	}
	m_hovered = is_collision;
}

void Button::mouseDownEvent() {
	if (m_hovered) {
		if (m_on_pressed) {
			m_on_pressed(m_on_pressed_args, this);
		}
		std::cout << "Clicked button" << std::endl;
	}
}