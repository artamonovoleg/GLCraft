#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    private:
        unsigned int m_ID;
        void CheckCompileErrors(unsigned int shader, std::string type);
    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        void Bind();
        void Unbind();
        
        void SetInt(const std::string &name, int value) const;
        void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};