#include "UVSphere.h"
#include "Utilities.h"
#include "Shader.h"

#include "gtc/constants.hpp"

UVSphere::UVSphere(const float radiusArg, const glm::vec3& positionArg, const uint32_t latitudeDivision, const uint32_t longitudeDivision)
	: radius(radiusArg)
	, position(positionArg)
{
	//positions.resize((latitudeDivision + 1) * (longitudeDivision + 1));
	normals.resize((latitudeDivision + 1) * (longitudeDivision + 1));
	texcoords.resize((latitudeDivision + 1) * (longitudeDivision + 1));
	indices.resize(latitudeDivision * longitudeDivision * 6);

	//const float radiusInverse = 1.0f / radius;

	uint32_t curr{ 0 };
	//for (uint32_t lon = 0; lon <= longitudeDivision; ++lon)
	//{
	//	const float u = float(lon) / float(longitudeDivision);
	//	const float currentLongitude = u * glm::pi<float>() * 2;

	//	const float sinLongitude = sinf(currentLongitude);
	//	const float cosLongitude = cosf(currentLongitude);

	//	for (uint32_t lat = 0; lat <= latitudeDivision; ++lat)
	//	{
	//		const float v = float(lat) / float(latitudeDivision);
	//		const float currentLatitude = (v - 0.5f) * glm::pi<float>();

	//		const float sinLatitude = sinf(currentLatitude);
	//		const float cosLatitude = cosf(currentLatitude);

	//		const float pos_x = cosLongitude * cosLatitude /** radius*/;
	//		const float pos_y = sinLatitude /** radius*/;
	//		const float pos_z = sinLongitude * cosLatitude /** radius*/;

	//		positions[curr] = glm::vec3{ pos_x, -pos_z, pos_y } /** radius*/;
	//		texcoords[curr] = { u,v };
	//		normals[curr] = positions[curr] /** radiusInverse*/;

	//		curr++;
	//	}
	//}

	for (uint32_t y = 0; y <= latitudeDivision; ++y)
	{
		for (uint32_t x = 0; x <= longitudeDivision; ++x)
		{
			const float u = float(x) / float(longitudeDivision);
			const float v = float(y) / float(latitudeDivision);

			const float theta = 2 * glm::pi<float>() * u;
			const float phi = glm::pi<float>() * (v - 0.5f);

			const float sin_theta = sinf(theta);
			const float cos_theta = cosf(theta);
			const float sin_phi = sinf(phi);
			const float cos_phi = cosf(phi);

			const float ux = cos_theta * cos_phi;
			const float uy = sin_phi;
			const float uz = cos_phi * sin_theta;

			//positions[curr] = { ux, -uz, uy };
			//texcoords[curr] = { u,v };
			//normals[curr] = positions[curr];

			positions2.push_back(ux);
			positions2.push_back(-uz);
			positions2.push_back(uy);
			positions2.push_back(ux);
			positions2.push_back(-uz);
			positions2.push_back(uy);
			positions2.push_back(-u);
			positions2.push_back(v);

			curr++;
		}
	}

	curr = 0;
	for (uint32_t lon = 0; lon < longitudeDivision; ++lon)
	{
		for (uint32_t lat = 0; lat < latitudeDivision; ++lat)
		{
			indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon;
			indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon;
			indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon + 1;

			indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon;
			indices[curr++] = (lat + 1) * (longitudeDivision + 1) + lon + 1;
			indices[curr++] = (lat + 0) * (longitudeDivision + 1) + lon + 1;
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, positions2.size() * sizeof(float), &positions2[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	shader = new Shader(getShaderPath("sphere.vs"), getShaderPath("sphere.fs"));
}

UVSphere::~UVSphere()
{
	delete shader;
}

void UVSphere::draw(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos, const GLuint texture) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(shader->getID());

	shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	shader->setVec3("viewPos", viewPos);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, GLsizei(positions2.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}