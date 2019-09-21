#include "Converter.h"
#include "Coordinate.h"
#include "MercatorMath.h"
#include "Sphere.h"
#include "Utilities.h"
#include "Shader.h"

#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float& mixer)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixer += 0.01f;

		if (mixer > 1.0f)
		{
			mixer = 1.0f;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixer -= 0.01f;

		if (mixer < 0.0f)
		{
			mixer = 0.0f;
		}
	}
}

GLuint loadTexture(const std::string& name, const int format)
{
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

int main()
{
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	stbi_set_flip_vertically_on_load(true);


	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3
	};

	
	Shader sh(getShaderPath("test.vs"), getShaderPath("test.fs"));

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	const GLuint texture1 = loadTexture("container.jpg", GL_RGB);
	const GLuint texture2 = loadTexture("wall.jpg", GL_RGB);
	const GLuint texture3 = loadTexture("awesomeface.png", GL_RGBA);

	sh.use();

	float mixerFloat = 0.0f;

	unsigned int inc = 0;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, mixerFloat);

		glm::mat4 trans(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5, 0.5, 0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0));
		trans = glm::rotate(trans, glm::radians(float(inc++)), glm::vec3(0, 0, 1));
		

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture3);
		//glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		sh.setMat4("transform", trans);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);
		


		glm::mat4 trans2(1.0f);
		trans2 = glm::translate(trans2, glm::vec3(-0.5, -0.5, 0));
		trans2 = glm::scale(trans2, glm::vec3(0.5, 0.5, 0));
		trans2 = glm::rotate(trans2, glm::radians(float(inc++)) * -1, glm::vec3(0, 0, 1));

		sh.setMat4("transform", trans2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	const glm::vec3 sunPosition{ 10, 10, 20 };
	const Sphere earth{ {0,0,0}, 5 };

	const glm::vec3 earthToSun = sunPosition - earth.center;

	constexpr int longitudeDivision = 100;
	constexpr int latitudeDivision = 40;

	CoordinateLonLat c1{ -math::PI, 0 };
	CoordinateLonLat c2{ math::PI, 0 };
	CoordinateLonLat c3{ 0, math::PI_HALF - 0.0001f };	//small number added because of infinity problems
	CoordinateLonLat c4{ 0, -math::PI_HALF + 0.0001f };

	c1.print();
	c2.print();
	c3.print();
	c4.print();

	const CoordinateXY test1 = Converter::ProjectToPlane(c1, earth.radius);
	const CoordinateXY test2 = Converter::ProjectToPlane(c2, earth.radius);
	const CoordinateXY test3 = Converter::ProjectToPlane(c3, earth.radius);
	const CoordinateXY test4 = Converter::ProjectToPlane(c4, earth.radius);

	const float xmin = test1.x;
	const float xmax = test2.x;
	const float ymin = test4.y;
	const float ymax = test3.y;

	Converter::ProjectToSphere(test1, 1).print();
	Converter::ProjectToSphere(test2, 1).print();
	Converter::ProjectToSphere(test3, 1).print();
	Converter::ProjectToSphere(test4, 1).print();

	const float xIncrement = float(xmax - xmin) / longitudeDivision;
	const float yIncrement = float(ymax - ymin) / latitudeDivision;
	

	int** arrays = new int*[latitudeDivision];
	for (int i = 0; i < latitudeDivision; ++i)
	{
		arrays[i] = new int[longitudeDivision];
	}

	for (int i = 0; i < latitudeDivision; ++i)
	{
		const float currentY = ymin + i * yIncrement;
		for (int j = 0; j < longitudeDivision; ++j)
		{
			const float currentX = xmin + j * xIncrement;

			const CoordinateLonLat currentLonLat = Converter::ProjectToSphere({ currentX, currentY }, earth.radius);
			const glm::vec3 currentNormal = currentLonLat.toDirectionVector();
			const float dot = glm::dot(currentNormal, earthToSun);

			//std::cout << currentX << " " << currentY << "\n";
			//currentLonLat.print();
			//printvec(currentNormal);
			//std::cout << dot << "\n\n";

			if (dot < 0)
			{
				arrays[i][j] = 0;
			}
			else
			{
				arrays[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < latitudeDivision; ++i)
	{
		for (int j = 0; j < longitudeDivision; ++j)
		{
			std::cout << arrays[i][j];
		}

		std::cout << "\n";
	}


	//constexpr float longitudeIncrement = math::PI_2 / float(longitudeDivision);
	//constexpr float latitudeIncrement = math::PI / float(latitudeDivision);

	

	

	//int** arrays = new int*[latitudeDivision];
	//for (int i = 0; i < latitudeDivision; ++i)
	//{
	//	arrays[i] = new int[longitudeDivision];
	//}

	//for (int i = 0; i < latitudeDivision; ++i)
	//{
	//	for (int j = 0; j < longitudeDivision; ++j)
	//	{
	//		const float currentLon = (j / float(longitudeDivision)) * math::PI_2;
	//		const float currentLat = (i / float(latitudeDivision)) * math::PI - math::PI_HALF;

	//		const CoordinateLonLat currentCoord{ currentLon, currentLat };

	//		const glm::vec3 normal = currentCoord.toDirectionVector();
	//		if (glm::dot(normal, earthToSun) < 0)
	//		{
	//			arrays[i][j] = 0;
	//		}
	//		else
	//		{
	//			arrays[i][j] = 1;
	//		}
	//	}
	//}

	//for (int i = 0; i < latitudeDivision; ++i)
	//{
	//	for (int j = 0; j < longitudeDivision; ++j)
	//	{
	//		std::cout << arrays[i][j];
	//	}

	//	std::cout << "\n";
	//}
	//for (float lon = 0; lon < math::PI_2; lon += longitudeIncrement)
	//{
	//	for (float lat = -math::PI_HALF; lat < math::PI_HALF; lat += latitudeIncrement)
	//	{
	//		CoordinateLonLat test{ lon, lat };

	//		const glm::vec3 normal = test.toDirectionVector();
	//		if (glm::dot(normal, earthToSun) < 0)
	//		{
	//		}
	//		//test.print();
	//		//printvec(test.toDirectionVector());
	//		//const auto result = Converter::ProjectToPlane(test, earth.radius);
	//		//result.print();
	//	}
	//}

	//for (float i = -math::PI_HALF - 0.1; i < math::PI_HALF + 0.1; i+=0.01)
	//{
	//	CoordinateLonLat test{ 1, i };
	//	test.print();

	//	const auto result = Converter::ProjectToPlane(test, 1);

	//	result.print();
	//}
}