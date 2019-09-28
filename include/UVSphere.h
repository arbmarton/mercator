#pragma once

#include "glm.hpp"
#include <glad/glad.h>

#include <vector>

class Shader;

class UVSphere
{
public:
	UVSphere(const float radiusArg, const glm::vec3& positionArg, const uint32_t latitudeDivision, const uint32_t longitudeDivision);
	~UVSphere();

	void draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const GLuint texture) const;

	glm::vec3 getPosition() const
	{
		return position;
	}

private:
	const float radius;
	glm::vec3 position;

	std::vector<float> positions2;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<uint32_t> indices;

	Shader* shader;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};