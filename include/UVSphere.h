#pragma once

#include "Utilities.h"

#include "glm.hpp"
#include <glad/glad.h>

#include <vector>

class Shader;

class UVSphere
{
public:
	UVSphere(const float radiusArg, const glm::vec3& positionArg, const uint32_t latitudeDivision, const uint32_t longitudeDivision);
	~UVSphere();

	void draw(/*const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const glm::vec3& lightColor, const glm::vec3& lightPos*/) const;

	glm::vec3 getPosition() const
	{
		return position;
	}

	Shader* getShader() const
	{
		return shader;
	}

private:
	const GLuint earthTexture = loadTexture("earth2048.bmp", GL_RGB);

	const float radius;
	glm::vec3 position;

	std::vector<float> vertexData;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<uint32_t> indices;

	Shader* shader{ nullptr };

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};