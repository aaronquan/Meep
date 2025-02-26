#include "text_input.hpp"

glm::mat4 TextInput::m_projection = glm::mat4(1.0f);
TextInput* TextInput::m_highlighted_input = nullptr;

bool TextInput::m_cursor_display = false;
float TextInput::m_cursor_current_time = 0.0f;
float TextInput::m_cursor_toggle_time = 0.6f;

TextInput::TextInput() : TextInput(10, 10, 10, 10) {};

TextInput::TextInput(float x, float y, float width, float height, float scale) :
	m_width(width), m_height(height),
	m_x(x), m_y(y), m_is_highlighted(false),
	m_text_scale(height / DrawText::default_size),
	m_text_width(0),
	m_cursor_position(0),
	m_collision(x, y, width, height),
	m_on_change(),
	m_on_change_args(nullptr),
	m_char_limit(1000)
{};

void TextInput::draw(ShaderLibrary::MVPShader& shader) const {
	drawBackground(shader);
	drawText();
}

void TextInput::drawText() const {
	DrawText::renderText(m_string, m_x, m_y, m_text_scale);
}

void TextInput::drawBackground(ShaderLibrary::MVPShader& shader) const {
	glm::mat4 mvp(1.0f);
	glm::mat4 model(1.0f);

	model = glm::translate(model, glm::vec3(m_x + m_width / 2.0f, m_y + m_height / 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(m_width, m_height, 1.0f));
	mvp = m_projection * model;
	shader.use();
	shader.setMVP(mvp);

	Quad::draw();
}

std::string TextInput::getString() const {
	return m_string;
}

CollisionRect TextInput::getCollision() const {
	return m_collision;
}

std::optional<Vector2D> TextInput::collisionPoint(float x, float y){
	int dx = x - m_x;
	int dy = y - m_y;
	if (dx >= 0 && dx <= m_width && dy >= 0 && dy <= m_height) {
		size_t new_cursor_position = searchWidthPositionIndex(dx);
		TextInput::resetCursorTimer();
		m_cursor_position = new_cursor_position;
		return Vector2D(dx, dy);
	}
	return std::nullopt;
}

bool TextInput::isHighlighted() const {
	return m_is_highlighted;
}

float TextInput::getCharacterPosition() const {
	if (m_character_positions.size() == 0 || m_cursor_position == 0) {
		return 0;
	}
	return m_character_positions[m_cursor_position-1];
}

float TextInput::getX() const {
	return m_x;
}

float TextInput::getY() const {
	return m_y;
}

void TextInput::setPosition(float x, float y) {
	m_x = x; m_y = y;
	std::cout << x << '\n';
	m_collision = CollisionRect(x, y, m_width, m_height);
}

void TextInput::addCharacter(char c, bool trigger_on_change) {
	TextInput::resetCursorTimer();
	float char_width = getCharacterWidth(c);

	//insert character into string
	m_string.insert(m_cursor_position, 1, c);

	//shift character positions behind
	std::vector<float>& character_positions = m_character_positions;
	for (std::vector<float>::iterator it = character_positions.begin() + m_cursor_position; it != m_character_positions.end(); ++it) {
		*it += char_width;
	}
	float position = char_width + getCharacterPosition();
	m_character_positions.insert(m_character_positions.begin() + m_cursor_position, position);
	m_cursor_position += 1;

	if (trigger_on_change && m_on_change != nullptr) {
		m_on_change(m_on_change_args, this);
	}
	else {
		std::cout << "on change null" << '\n';
	}
}

void TextInput::addBackspace() {
	if (m_string.size() > 0 && m_cursor_position > 0) {
		size_t delete_position = m_cursor_position - 1;
		char deleted_char = m_string[delete_position];
		m_string.erase(delete_position, 1);
		m_character_positions.erase(m_character_positions.begin() + delete_position);
		float char_width = TextInput::getCharacterWidth(deleted_char);
		m_text_width -= char_width;
		for (std::vector<float>::iterator it = m_character_positions.begin() + delete_position; it != m_character_positions.end(); ++it) {
			*it -= char_width;
		}

		m_cursor_position -= 1;
		//printCharacterPositions();
		if (m_on_change != nullptr) {
			m_on_change(m_on_change_args, this);
		}
	}
}

void TextInput::moveLeft() {
	if (m_cursor_position > 0) {
		m_cursor_position -= 1;
	}
}

void TextInput::moveRight() {
	if (m_cursor_position < m_string.size()) {
		m_cursor_position += 1;
	}
}

void TextInput::clear() {
	m_cursor_position = 0;
	m_string = "";
	m_character_positions.clear();
}

void TextInput::setText(std::string s) {
	clear();
	for (char c : s) {
		//std::cout << c << '\n';
		addCharacter(c, false);
	}
}

void TextInput::setCharLimit(unsigned int limit) {
	if (m_string.size() > limit) {
		m_string.erase(m_string.begin() + limit);
	}
	m_char_limit = limit;
}

void TextInput::setOnChange(std::function<void(void*, TextInput*)> f, void* args) {
	m_on_change = f;
	m_on_change_args = args;
}

bool TextInput::addCharacterToHighlighted(char c) {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->addCharacter(c);
		return true;
	}
	return false;
}

bool TextInput::addBackspaceToHighlighted() {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->addBackspace();
		return true;
	}
	return false;
}

bool TextInput::moveHighlightedLeft() {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->moveLeft();
		return true;
	}
	return false;
}

bool TextInput::moveHighlightedRight() {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->moveRight();
		return true;
	}
	return false;
}

TextInput* TextInput::getHighlightedInput() {
	return m_highlighted_input;
}

std::optional<std::string> TextInput::getHighlightedText() {
	if (m_highlighted_input != nullptr) {
		return m_highlighted_input->m_string;
	}
	return std::nullopt;
}

void TextInput::setHighlightedInput(TextInput* ti) {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->m_is_highlighted = false;
	}
	ti->m_is_highlighted = true;
	m_highlighted_input = ti;
}

void TextInput::noHighlightedInput() {
	if (m_highlighted_input != nullptr) {
		m_highlighted_input->m_is_highlighted = false;
	}
	m_highlighted_input = nullptr;
}

void TextInput::setWindow(int width, int height) {
	m_projection = glm::mat4(1.0f);
	m_projection = glm::ortho(0.0f, static_cast<float>(width), 
		static_cast<float>(height), 0.0f,
		std::numeric_limits<float>::min(),
		std::numeric_limits<float>::max());
}

void TextInput::drawHighlightedCursor(ShaderLibrary::MVPShader& shader) {

	if (m_cursor_display && m_highlighted_input != nullptr) {
		float x = m_highlighted_input->m_x + m_highlighted_input->getCharacterPosition();
		float y = m_highlighted_input->m_y;
		float height = m_highlighted_input->m_height;
		shader.use();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(x, y + height / 2, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, height, 1.0f));
		glm::mat4 mvp = m_projection * model;
		shader.setMVP(mvp);
		Quad::draw();
	}
}

void TextInput::cursorTick(float dt) {
	m_cursor_current_time += dt;
	if (m_cursor_current_time > m_cursor_toggle_time) {
		m_cursor_current_time = 0.0f;
		m_cursor_display = !m_cursor_display;
	}
}

size_t TextInput::searchWidthPositionIndex(float x) const {
	if (x < 0 || m_character_positions.size() == 0) {
		return 0;
	}
	if (x > m_character_positions[m_character_positions.size() - 1]) {
		return m_character_positions.size();
	}
	
	size_t i = 0;
	size_t j = m_character_positions.size() - 1;
	while (i <= j) {
		size_t mid = (i + j) >> 1;
		if (x == m_character_positions[mid]) {
			return mid+1;
		}
		if (x > m_character_positions[mid]) {
			i = mid+1;
		}
		else {
			if (mid == 0) {
				break;
			}
			j = mid-1;
		}
	}
	if (i == 0) {
		if (x + x < m_character_positions[0]) {
			return 0;
		}
		return 1;
	}
	if (x - m_character_positions[i-1] > m_character_positions[i] - x) {
		return i+1;
	}
	return i;
}

float TextInput::getCharacterWidth(char c) {
	std::optional<Character> char_op = DrawText::getCharacter(c);
	if (char_op.has_value()) {
		Character character = char_op.value();
		return (static_cast<float>(character.Advance) / 64.0f) * m_text_scale;
	}
	return 0;
}

void TextInput::printCharacterPositions() const {
	for (std::vector<float>::const_iterator it = m_character_positions.begin(); it != m_character_positions.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << '\n';
}

void TextInput::resetCursorTimer() {
	m_cursor_current_time = 0;
	m_cursor_display = true;
}

NumberInput::NumberInput(float x, float y, float width, float height,
	float scale, bool includes_decimal)
: TextInput(x, y, width, height, scale), m_includes_decimal(includes_decimal)
{
	//setText("0");
};

void NumberInput::addCharacter(char c, bool trigger_on_change) {
	std::cout << "Filtering" << std::endl;
	if (filterInput(c)) {
		TextInput::addCharacter(c, trigger_on_change);
	}
}

float NumberInput::getValueFloat() const {
	if (m_string.empty()) return 0.0f;
	if (m_string.size() == 1 && (m_string[0] == '-' || m_string[0] == '.')) {
		return 0.0f;
	}
	return std::stof(m_string);
}

int NumberInput::getValueInt() const {
	if (m_string.empty()) return 0;
	if (m_string.size() == 1 && (m_string[0] == '-' || m_string[0] == '.')) {
		return 0;
	}
	return std::stoi(m_string);
}

void NumberInput::setValue(int v) {
	setText(std::to_string(v));
}

void NumberInput::setValue(float f) {
	setText(std::to_string(f));
}

bool NumberInput::filterInput(char c) {
	if (isdigit(c)) {
		return true;
	}
	else {
		if (c == '-' && m_cursor_position == 0 && !m_string.starts_with("-")) {
			return true;
		}
		if (c == '.' && m_includes_decimal) {
			if (m_string.find('.') == -1) {
				return true;
			}
		}
	}
	return false;
}

void NumberInput::fixText() {
	if (isUnfinished()) {
		m_string = "0";
		return;
	}
	//bool in_decimal = false;
	for (size_t i = 0; i < m_string.size(); ++i) {
		if (m_string[i] != '0') {
			break;
		}
		if (i + 1 < m_string.size() && m_string[i + 1] == '.') {
			m_string.erase(0);
		}

	}
}

bool NumberInput::isUnfinished() const {
	if (m_string.empty()) return true;
	if (m_string.size() == 1 && (m_string[0] == '-' || m_string[0] == '.')) {
		return true;
	}
	if (m_string[0] == '.') {
		for (size_t i = 1; i < m_string.size(); ++i) {
			if (m_string[i] != '0') {
				return false;
			}
		}
		return true;
	}
	return false;
}
