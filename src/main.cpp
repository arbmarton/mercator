#include "Converter.h"
#include "Coordinate.h"
#include "MercatorMath.h"
#include "Sphere.h"
#include "Utilities.h"
#include "Shader.h"
#include "Camera.h"
#include "UVSphere.h"
#include "DisplayMap.h"
#include "StellarObject.h"

#include "stb_image.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>


bool firstMouse = true;
Camera camera;

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera& cam)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.moveForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.moveBackward();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.moveLeft();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.moveRight();
	}
}

void mouse_callback(GLFWwindow* window, const double xpos, const double ypos)
{
	if (firstMouse)
	{
		CursorPosDescriptor::instance().lastX = float(xpos);
		CursorPosDescriptor::instance().lastY = float(ypos);

		firstMouse = false;
	}

	const float xOffset = float(xpos) - CursorPosDescriptor::instance().lastX;
	const float yOffset = CursorPosDescriptor::instance().lastY - float(ypos);
	CursorPosDescriptor::instance().lastX = float(xpos);
	CursorPosDescriptor::instance().lastY = float(ypos);


	camera.processMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	constexpr float maxFov = 90.0f;
	constexpr float minFov = 1.0f;

	if (camera.getFov() >= minFov && camera.getFov() <= maxFov)
	{
		camera.setFov(camera.getFov() - float(yoffset));
	}
	if (camera.getFov() <= minFov)
	{
		camera.setFov(minFov);
	}
	if (camera.getFov() >= maxFov)
	{
		camera.setFov(maxFov);
	}
}

int main()
{
	CursorPosDescriptor::instance().lastX = ScreenDescriptor::WINDOW_WIDTH / 2;
	CursorPosDescriptor::instance().lastY = ScreenDescriptor::WINDOW_HEIGHT / 2;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(ScreenDescriptor::WINDOW_WIDTH, ScreenDescriptor::WINDOW_HEIGHT, "Lit Earth", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	DisplayMap map;
	
	StellarObject earthObject = StellarObject({ 0, 0, 0 }, 1, "sphere", "earth2048.bmp", GL_RGB, false);
	StellarObject sunObject = StellarObject({ 0, 0, 0 }, 1, "sphere", "2k_sun.jpg", GL_RGB, true);

	earthObject.getRotation() = glm::rotate(glm::mat4(1.0f), glm::radians(/*-23.5f*/ 0.0f), { 0,0,1 });

	const glm::vec3 lightColor{ 1,1,1 };

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// Update
		const float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		sunObject.getPosition() = { 50 * float(cos(glfwGetTime())), 0, 50 * float(sin(glfwGetTime())) };

		camera.setSpeed(deltaTime * 5.0f);
		processInput(window, camera);

		// Draw
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		map.draw(earthObject.getRadius(), sunObject.getPosition() - earthObject.getPosition());
		sunObject.draw(camera, lightColor, sunObject.getPosition());
		earthObject.draw(camera, lightColor, sunObject.getPosition());

		// Upkeep
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