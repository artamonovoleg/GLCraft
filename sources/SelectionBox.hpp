#pragma once

#include <memory>
#include "RaycastResult.hpp"

class VertexArray;
class VertexBuffer;
class Shader;
class Camera;

class SelectionBox
{
    private:
        std::shared_ptr<Shader>         m_Shader;
        
        std::shared_ptr<VertexArray>            m_VA;
        std::shared_ptr<VertexBuffer>           m_VB;
    public:
        SelectionBox();
        void Draw(const Camera& camera, const RaycastResult& result);
};