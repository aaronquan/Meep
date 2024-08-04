#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <vector>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


class Shader
{
public:
    // the program ID
    unsigned int ID;
    Shader();
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat3(const std::string& name, float v1, float v2, float v3) const;
    void setFloat4(const std::string& name, float v1, float v2, float v3, float v4) const;
    void setMatrix4(const std::string& name, const glm::mat4& mat) const;

    void addFragment(const char* fragmentPath);
    void addVertex(const char* vertexPath);
    void link();
private:
    bool m_is_setup;
    bool m_is_init;
    std::vector<unsigned int> shader_ids;
    void checkCompileErrors(unsigned int shader, std::string type);
    void initId();
};

#endif