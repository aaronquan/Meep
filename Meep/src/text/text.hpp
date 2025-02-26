#pragma once
#include <iostream>
#include <optional>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "./../math/matrix.hpp"
#include "./../shaders/shader.hpp"
#include "./../shaders/library.hpp"
#include "./../utils/colour.hpp"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

struct CharacterBufferData {
    float inverted_data[16];
    float normal_data[16];
};

class DrawText {
public:
	DrawText(){};

	static void setup(float width, float height);

    static void setupMVPShader();

    static void clean();
    static void setProjection(glm::mat4 projection);

    static bool loadCharacters();

    static void renderText(std::string text, float x, float y, 
    float scale=1.0f, Colour col=Colour());

    static void renderTextMVP(std::string text, const glm::mat4& mvp);
    static void renderTextMVP(std::string text, const glm::mat4& mvp, const Colour& col);

    static std::optional<Character> getCharacter(char c);

    static unsigned int default_size;

    static float drawCharacterMVP(char c, float x, float y);
    static float drawCharacterTest(char c, float x, float y, float scale);


    static float textWidth(std::string text, float scale);
    static float textWidth(std::string text, const glm::mat4& mvp);
    static float textHeight(int lines, float scale);
private:
    //static float drawCharacterMVP(char c, float x, float y);
    static float drawCharacter(char c, float x, float y, float scale); 

    static std::map<char, Character> m_characters;
    static bool s_characters_loaded;

    static Shader::MainShader* s_shader;
    static std::optional<Shader::MainShader> s_mvp_shader;
    static bool s_shader_loaded;

    static float projection_width;
    static float projection_height;

	static unsigned int VAO;
	static unsigned int VBO;
    //static unsigned int EBO;

    static unsigned int VAO2;
    static unsigned int VBO2;
    static unsigned int EBO2;

    static bool s_is_ready;

    static std::map<char, CharacterBufferData> s_character_buffer_data;
    //static std::map<char, float[6][4]> s_characters_invert_y;
    //static bool s_mvp_shader_ready;
};