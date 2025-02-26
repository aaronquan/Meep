#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include  <functional>
#include <optional>

#include "text.hpp"

#include "../math/vector2d.hpp"

#include "../shaders/shader.hpp"
#include "../shaders/library.hpp"
#include "../shapes/shapes.hpp"

class TextInput {
public:
	TextInput();
	TextInput(float x, float y, float width, float height, float scale=1.0f);
	void draw(ShaderLibrary::MVPShader& shader) const;
	void drawText() const;
	void drawBackground(ShaderLibrary::MVPShader& shader) const;

	std::string getString() const;

	CollisionRect getCollision() const;

	std::optional<Vector2D> collisionPoint(float x, float y);

	bool isHighlighted() const;
	float getCharacterPosition() const;

	float getX() const;
	float getY() const;
	void setPosition(float x, float y);

	virtual void addCharacter(char c, bool trigger_on_change=true);
	void addBackspace();
	void moveLeft();
	void moveRight();
	void clear();
	void setCharLimit(unsigned int limit);
	void setText(std::string s);

	void setOnChange(std::function<void(void*, TextInput*)> f, void* args=nullptr);

	//void drawCursor(ShaderLibrary::MVPShader& shader); //could implement cursor for multiple text inputs

	static bool addCharacterToHighlighted(char c);
	static bool addBackspaceToHighlighted();
	static bool moveHighlightedLeft();
	static bool moveHighlightedRight();

	static TextInput* getHighlightedInput();
	static std::optional<std::string> getHighlightedText();

	static void setHighlightedInput(TextInput* ti);
	static void noHighlightedInput();
	static void setWindow(int width, int height);

	static void drawHighlightedCursor(ShaderLibrary::MVPShader& shader);
	static void cursorTick(float dt);
protected:
	std::string m_string;
	size_t m_cursor_position;

	virtual void fixText() {}; //unused // change the text
private:
	size_t searchWidthPositionIndex(float x) const;
	void printCharacterPositions() const;
	float getCharacterWidth(char c);

	static void resetCursorTimer();

	unsigned int m_char_limit;

	float m_width;
	float m_height;
	float m_x;
	float m_y;
	float m_text_scale;
	float m_text_width;

	void* m_on_change_args;

	CollisionRect m_collision;

	std::vector<float> m_character_positions;

	bool m_is_highlighted;

	std::function<void(void*, TextInput*)> m_on_change;

	static TextInput* m_highlighted_input;
	static glm::mat4 m_projection;

	static bool m_cursor_display;
	static float m_cursor_current_time;
	static float m_cursor_toggle_time;

	//static ShaderLibrary::CoordinateShader m_background_shader;
};

class NumberInput: public TextInput {
public:
	NumberInput(float x, float y, float width, float height,
		float scale = 1.0f, bool includes_decimal=false);
	void addCharacter(char c, bool trigger_on_change=true);
	float getValueFloat() const;
	int getValueInt() const;
	void setValue(int v);
	void setValue(float f);
	void fixText();
private:
	bool filterInput(char c);
	bool isUnfinished() const;
	bool m_includes_decimal;
};