#pragma once

#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

class Texture
{
    private:
        unsigned int m_ID;
    public:
        Texture(const std::string& filename)
        {
            glGenTextures(1, &m_ID);
            glBindTexture(GL_TEXTURE_2D, m_ID);
            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // load image, create texture and generate mipmaps
            int width, height, nrChannels;
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
                std::cout << "Failed to load texture" << std::endl;
            stbi_image_free(data);
            Bind();
        }

        void Bind(int index = 0)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_ID);
        }

        void Unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
};