#include "DisplayMap.h"

#include "Shader.h"

DisplayMap::DisplayMap()
	: shader(new Shader(getShaderPath("displaymap.vs"), getShaderPath("displaymap.fs")))
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(displayVertices), displayVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(displayIndices), displayIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
}

void DisplayMap::draw() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earthTexture);

	glUseProgram(shader->getID());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(displayIndices) / 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}