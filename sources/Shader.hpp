#pragma once

#include <string>

class Shader
{
    private:
        unsigned int m_ID = 0;

        const char* ReadFromSource(const std::string& file);
        void CheckCompileStatus(unsigned int shader);
    public:
        Shader(const std::string& vertPath, const std::string& fragPath);
        ~Shader();
        
        void Bind();
        void Unbind();
};