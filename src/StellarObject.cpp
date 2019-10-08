#include "StellarObject.h"

#include "Shader.h"
#include "UVSphere.h"
#include "Utilities.h"
#include "Camera.h"

#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

StellarObject::StellarObject(const glm::vec3& pos, const float radius, const std::string& shaderName, const std::string& textureName, const int textureFormat)
	: sphere(new UVSphere(radius, pos, 100, 100))
	, shader(new Shader(getShaderPath(shaderName + ".vs"), getShaderPath(shaderName + ".fs")))
	, texture(loadTexture(textureName, textureFormat))
	, position(pos)
{
}

StellarObject::~StellarObject()
{
	delete sphere;
	delete shader;
}

float StellarObject::getRadius() const
{
	return sphere->getRadius();
}

void StellarObject::draw(const Camera& camera, const glm::vec3& lightColor, const glm::vec3& lightPos) const
{
	glUseProgram(shader->getID());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader->setMat4("model", glm::translate(glm::mat4(1.0f), position) * rotation);
	shader->setMat4("view", camera.getLookAt());
	shader->setMat4("projection", glm::perspective(glm::radians(camera.getFov()), ScreenDescriptor::WINDOW_WIDTH / float(ScreenDescriptor::WINDOW_HEIGHT), 0.1f, 1000.0f));
	shader->setVec3("viewPos", camera.getPosition());
	shader->setVec3("lightColor", lightColor);
	shader->setVec3("lightPos", lightPos);
	
	sphere->draw();
}