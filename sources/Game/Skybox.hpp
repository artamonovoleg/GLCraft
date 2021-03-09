#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class VertexArray;
class VertexBuffer;
class Shader;
class TextureCubemap;

class Skybox
{
    private:
        std::shared_ptr<TextureCubemap> m_Texture;
        std::shared_ptr<Shader>         m_Shader;
        
        std::shared_ptr<VertexArray>            m_VA;
        std::shared_ptr<VertexBuffer>           m_VB;
        
        unsigned int m_VAO = 0, m_VBO = 0;
    public:
        Skybox(const std::string& folder, const std::string& ext);

        void Draw(const glm::mat4& proj, const glm::mat4& view);
};