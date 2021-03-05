#pragma once

#include <memory>

class VertexArray;
class VertexBuffer;
class Shader;

class Crosshair
{
    private:
        std::shared_ptr<Shader>         m_Shader;
        
        std::shared_ptr<VertexArray>            m_VA;
        std::shared_ptr<VertexBuffer>           m_VB;
    public:
        Crosshair();
        ~Crosshair() = default;

        void Draw();
};