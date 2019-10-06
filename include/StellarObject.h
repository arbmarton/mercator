#pragma once

#include "Camera.h"

#include "glm.hpp"
#include <glad/glad.h>

#include <string>

class UVSphere;
class Shader;

class StellarObject
{
public:
	StellarObject() = delete;
	StellarObject(const glm::vec3& pos, const float radius, const std::string& shaderName, const std::string& textureName, const int textureFormat);
	~StellarObject();

	glm::mat4& getRotation()
	{
		return rotation;
	}

	glm::vec3& getPosition()
	{
		return position;
	}

	float getRadius() const;

	void draw(const Camera& camera, const glm::vec3& lightColor, const glm::vec3& lightPos) const;

private:
	UVSphere* sphere;
	Shader* shader;
	GLuint texture;

	glm::vec3 position;
	glm::mat4 rotation{ glm::mat4(1.0f) };
};