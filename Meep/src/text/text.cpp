#include "text.hpp"
#include "../shapes/shapes.hpp"

std::map<char, Character>  DrawText::m_characters;
bool DrawText::s_characters_loaded;

Shader::MainShader* DrawText::s_shader = nullptr;

std::optional<Shader::MainShader> DrawText::s_mvp_shader = std::nullopt;
//bool DrawText::s_mvp_shader_ready = false;

bool DrawText::s_shader_loaded = false;

unsigned int DrawText::VAO;
unsigned int DrawText::VBO;

unsigned int DrawText::VAO2;
unsigned int DrawText::VBO2;
unsigned int DrawText::EBO2;

float DrawText::projection_width = 0;
float DrawText::projection_height = 0;

unsigned int DrawText::default_size = 20;
bool DrawText::s_is_ready = false;

std::map<char, CharacterBufferData> DrawText::s_character_buffer_data;
//std::map<char, float[6][4]> DrawText::s_characters_standard_y;
//std::map<char, float[6][4]> DrawText::s_characters_invert_y;

void DrawText::setup(float width, float height) {
    if (!s_characters_loaded) {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW); // to change

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        loadCharacters();
        s_characters_loaded = true;
    }

    if(!s_shader_loaded){
        s_shader = new Shader::MainShader();
        std::optional<Shader::VertexShader> vertex_shader = Shader::createVertexShaderFromFile("src/text/shaders/text.vert");
        std::optional<Shader::FragmentShader> fragment_shader = Shader::createFragmentShaderFromFile("src/text/shaders/text.frag");
        if (vertex_shader != std::nullopt && fragment_shader != std::nullopt) {
            s_shader->addVertex(vertex_shader.value());
            s_shader->addFragment(fragment_shader.value());
        }
        else {
            std::cout << "No text shader linked" << std::endl;
            return;
        }

        s_shader->link();
        glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
        //glm::mat4 model(1.0f);//
        //model = glm::scale(model, glm::vec3(1.2, 1, 1));//
        //projection *= model;//
        s_shader->use();
        s_shader->setMat4("mvp", projection);
        projection_width = width;
        projection_height = height;

        s_shader_loaded = true;

    }

    if (s_characters_loaded && s_shader_loaded) {
        s_is_ready = true;
    }
}

void DrawText::setupMVPShader() {
    if (!s_characters_loaded) {

        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        //glBindVertexArray(0);

        loadCharacters();
        s_characters_loaded = true;
    }

    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    s_mvp_shader = Shader::MainShader();
    Shader::MainShader& shader = s_mvp_shader.value();
    Shader::VertexShader vertex_shader = ShaderLibrary::generateVertexShaderFromPath("src/text/shaders/text.vert");
    Shader::FragmentShader fragment_shader = ShaderLibrary::generateFragmentShaderFromPath("src/text/shaders/text.frag");
    shader.addVertex(vertex_shader);
    shader.addFragment(fragment_shader);
    shader.link();
    shader.use();
    shader.setMat4("mvp", glm::mat4(1.0f));
}

void DrawText::clean() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void DrawText::setProjection(glm::mat4 projection) {
    s_shader->use();
    s_shader->setMat4("projection", projection);
}


bool DrawText::loadCharacters() {

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, "./src/text/fonts/arial.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, default_size);
    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        return false;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        m_characters.insert(std::pair<char, Character>(c, character));


        float xpos = character.Bearing.x;
        float ypos = (character.Size.y - character.Bearing.y + default_size);
        float w = character.Size.x;
        float h = character.Size.y;

        float inverted_vertices[16] = {
        xpos, ypos - h, 0.0f, 0.0f,
        xpos, ypos, 0.0f, 1.0f,
        xpos + w, ypos, 1.0f, 1.0f,
        xpos + w, ypos - h, 1.0f, 0.0f
        };

        float normal_vertices[16] = {
        xpos, ypos + h, 0.0f, 0.0f,
        xpos, ypos, 0.0f, 1.0f,
        xpos + w, ypos, 1.0f, 1.0f,
        xpos + w, ypos + h, 1.0f, 0.0f
        };
        CharacterBufferData cba;
        for (size_t i = 0; i < 16; i++) {
            cba.inverted_data[i] = inverted_vertices[i];
        }
        //cba.inverted_data = inverted_vertices;
            
        s_character_buffer_data.insert(std::pair<char, CharacterBufferData>(c, cba));


        //s_characters_invert_y.insert(std::pair<char, float[6][4]>(c, invert_vertices));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return true;
}

//void DrawText::

void DrawText::renderText(std::string text, float x, float y, float scale, Colour col) {
    if (s_is_ready) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_shader->use();
        glm::mat4 projection = glm::ortho(0.0f, projection_width, 0.0f, projection_height);
        s_shader->setMat4("mvp", projection);
        s_shader->setVec3("textColour", col.toVec3());

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;

        for (c = text.begin(); c != text.end(); c++)
        {
            x += drawCharacterTest(*c, x, y, scale);
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
    else {
        std::cout << "Text is not setup" << std::endl;
    }
}

//void DrawText::renderTextTest

void DrawText::renderTextMVP(std::string text, const glm::mat4& mvp) {
    Colour black = Colour();
    renderTextMVP(text, mvp, black);
}

void DrawText::renderTextMVP(std::string text, const glm::mat4& mvp, const Colour& col) {
    if (!s_mvp_shader.has_value()) {
        setupMVPShader();
    }
    Shader::MainShader& shader = s_mvp_shader.value();
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    //glBindVertexArray(VAO);
    shader.use();
    //glm::mat4 mvp1 = glm::ortho(0.0f, projection_width, 0.0f, projection_height);
    shader.setMat4("mvp", mvp);
    shader.setVec3("textColour", col.toVec3());
    //Quad::draw();
    //s_shader->use();
    //s_shader->setMat4("mvp", mvp);
    //s_shader->setVec3("textColour", col.toVec3());
    float x = 0;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        //std::cout << *c << ' ' << x << '\n';
        x += drawCharacterMVP(*c, x, 0);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}

std::optional<Character> DrawText::getCharacter(char c) {
    if (m_characters.find(c) != m_characters.end()) {
        return m_characters[c];
    }
    return std::nullopt;
}


float DrawText::drawCharacterMVP(char c, float x, float y) {
    
    Character ch = m_characters[c];
    CharacterBufferData cbd = s_character_buffer_data[c];
    /*
    float scale = 0.2f;
    float xpos = x + ch.Bearing.x * scale;
    float ypos = (ch.Size.y - ch.Bearing.y + default_size) * scale;
    //std::cout << xpos << ' ' << ypos << '\n';
    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    //std::cout << w << ' ' << h << '\n';
    
    float vertices[6][4] = {
        { xpos,     ypos - h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos - h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos - h,   1.0f, 0.0f }
    };

    float new_vertices[16] = {
        xpos, ypos-h, 0.0f, 0.0f,
        xpos, ypos, 0.0f, 1.0f,
        xpos+w, ypos, 1.0f, 1.0f,
        xpos+w, ypos-h, 1.0f, 0.0f
    };*/

    float new_vertices[16];
    for (size_t i = 0; i < 16; i++) {
        if (i % 4 == 0) {
            new_vertices[i] = cbd.inverted_data[i] + x;
        }
        else {
            new_vertices[i] = cbd.inverted_data[i];
        }
    }


    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(new_vertices), new_vertices);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cbd.inverted_data), cbd.inverted_data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(VAO2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    return (ch.Advance >> 6);
}

float DrawText::drawCharacter(char c, float x, float y, float scale) {
    Character ch = m_characters[c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = projection_height - y - (ch.Size.y - ch.Bearing.y + default_size) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    // update VBO for each character
    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    return (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
}

float DrawText::drawCharacterTest(char c, float x, float y, float scale) {
    Character ch = m_characters[c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = projection_height - y - (ch.Size.y - ch.Bearing.y + default_size) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    // update VBO for each character
    float vertices[6][4] = {
       { xpos,     ypos + h,   0.0f, 0.0f },
       { xpos,     ypos,       0.0f, 1.0f },
       { xpos + w, ypos,       1.0f, 1.0f },

       { xpos,     ypos + h,   0.0f, 0.0f },
       { xpos + w, ypos,       1.0f, 1.0f },
       { xpos + w, ypos + h,   1.0f, 0.0f }
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    return (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
}

float DrawText::textWidth(std::string text, float scale) {
    float width = 0;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        Character ch = m_characters[c];
        width += (ch.Advance >> 6) * scale;
    }
    return width;
}

float DrawText::textHeight(int lines, float scale) {
    return lines*scale*default_size;
}

//float DrawText::

float DrawText::textWidth(std::string text, const glm::mat4& mvp) {
    float width = 0;
    float scale_x = getScaleFromMatrix(mvp)[0];
    //std::cout << scale_x << '\n';

    for (size_t i = 0; i < text.size(); ++i){
        char c = text[i];
        Character ch = m_characters[c];
        width += (ch.Advance >> 6);
        //width += (ch.Advance >> 6)*scale_x;
        //std::cout << ch.Size[0] * scale_x;
    }
    //Character last = m_characters[text.back()];
    //width += last.Size[0] * scale_x;
    return width;
}