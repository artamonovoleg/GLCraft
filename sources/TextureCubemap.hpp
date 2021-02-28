#pragma once

#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

class TextureCubemap
{
    private:
        unsigned int m_ID;
    public:
        TextureCubemap(const std::vector<std::string>& faces)
        {
            glGenTextures(1, &m_ID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

            int width, height, nrComponents;
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                unsigned char *data = stbi_load(faces.at(i).c_str(), &width, &height, &nrComponents, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    std::cerr << "Cubemap texture failed to load at path: " << faces.at(i) << std::endl;
                    stbi_image_free(data);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        }

        ~TextureCubemap()
        {
            glDeleteTextures(1, &m_ID);
        }

        void Bind(int index = 0)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
        }

        void Unbind()
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
};