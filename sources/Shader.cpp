#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "Shader.hpp"

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
    : m_ID(0)
{
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto vertSrc = ReadFromSource(vertPath);
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);
    CheckCompileStatus(vertexShader);
    
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* s_c = ReadFromSource("../shaders/frag.glsl");
    glShaderSource(fragmentShader, 1, &s_c, NULL);
    glCompileShader(fragmentShader);
    CheckCompileStatus(fragmentShader);

    // link shaders
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

const char* Shader::ReadFromSource(const std::string& file)
{
    std::ifstream is(file);
    std::string s;
    std::string temp;

    while (std::getline(is, temp)) s += temp + "\n";

    return std::move(s.c_str());
}

void Shader::CheckCompileStatus(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compile failed" << std::endl;
    }
}

void Shader::Bind()
{
    glUseProgram(m_ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}