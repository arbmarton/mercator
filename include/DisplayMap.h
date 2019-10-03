#pragma once

#include "Utilities.h"

#include <glad/glad.h>

class Shader;

class DisplayMap
{
public:
	DisplayMap();

	void draw() const;

private:
	constexpr static float topLeftX = 0.4f;
	constexpr static float topLeftY = -0.4f;

	// screen space coordinates
	constexpr static float displayVertices[20] =
	{
		topLeftX, topLeftY, -1.0f, 0.0f, 1.0f, // top left
		topLeftX, -1.0f, -1.0f, 0.0f, 0.0f, // bottom left
		1.0f, -1.0f, -1.0f, 1.0f, 0.0f, // bottom right
		1.0f, topLeftY, -1.0f, 1.0f, 1.0f // top right
	};

	constexpr static int displayIndices[6] =
	{
		3, 2, 0,
		2, 1, 0
	};

	const GLuint earthTexture = loadTexture("earth2048.bmp", GL_RGB);

	Shader* shader{ nullptr };

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};