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


#include "./../shaders/shader.hpp"
#include "./../utils/colour.hpp"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class DrawText {
public:
	DrawText(){};

	static void setup(float width, float height);


    static void setProjection(glm::mat4 projection) {
        shader->use();
        shader->setMat4("projection", projection);
    }

    static void clean() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    static bool loadCharacters();

    static void renderText(std::string text, float x, float y, 
    float scale=1.0f, Colour col=Colour());

private:

    static std::map<char, Character> Characters;
    static bool s_characters_loaded;

    static Shader::MainShader* shader;
    static bool s_shader_loaded;


	static unsigned int VAO;
	static unsigned int VBO;

    static bool s_is_ready;
};