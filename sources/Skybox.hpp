#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Shader;
class TextureCubemap;

class Skybox
{
    private:
        std::shared_ptr<TextureCubemap> m_Texture;
        std::shared_ptr<Shader>         m_Shader;
        
        unsigned int m_VAO = 0, m_VBO = 0;
    public:
        Skybox(const std::vector<std::string>& faces);

        void Draw(const glm::mat4& proj, const glm::mat4& view);
};