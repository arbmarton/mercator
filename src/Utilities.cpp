#include "Utilities.h"

#include "stb_image.h"

std::filesystem::path getShaderFolderPath()
{
    std::string root = std::filesystem::current_path().parent_path().string();
    root.append("/shader/");
    return std::filesystem::path(root);
}

std::filesystem::path getShaderPath(const std::string& shaderName)
{
    return std::filesystem::path(getShaderFolderPath().string() + shaderName);
}

std::filesystem::path getTextureFolderPath()
{
    std::string root = std::filesystem::current_path().parent_path().string();
    root.append("/textures/");
    return std::filesystem::path(root);
}

std::filesystem::path getTexturePath(const std::string& shaderName)
{
    return std::filesystem::path(getTextureFolderPath().string() + shaderName);
}

GLuint loadTexture(const std::string& name, const int format)
{
    stbi_set_flip_vertically_on_load(true);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(getTexturePath(name).string().c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(texture);
    }
    else
    {
        std::cout << "Failed to load texture...\n";
        return 0;
    }

    stbi_image_free(data);

    return texture;
}