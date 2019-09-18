#pragma once

#include <glad/glad.h>

#include <filesystem>

class Shader
{
public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

	GLuint getID() const
	{
		return ID;
	}

	void use() const
	{
		glUseProgram(ID);
	}

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	GLuint ID;

	Shader() = delete;
	Shader(const Shader& rhs) = delete;
	Shader(Shader&& rhs) = delete;
};