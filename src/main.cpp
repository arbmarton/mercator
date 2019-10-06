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

	constexpr float earthAxisOffset = -23.5f;
	constexpr float sunDistance = 100;
	constexpr float rotationSpeed = 0.01f;
	constexpr float earthRadius = 1.0f;
	constexpr float sunRadius = 10.0f;

	DisplayMap map;
	
	StellarObject earthObject = StellarObject({ 0, 0, 0 }, earthRadius, "sphere", "earth2048.bmp", GL_RGB, false);
	StellarObject sunObject = StellarObject({ 0, 0, 0 }, sunRadius, "sphere", "2k_sun.jpg", GL_RGB, true);
	StellarObject starsObject = StellarObject(camera.getPosition(), sunDistance * 2, "sphere", "8k_stars_milky_way.jpg", GL_RGB, true);

	earthObject.getRotation() = glm::rotate(glm::mat4(1.0f), glm::radians(earthAxisOffset), { 0, 0, 1 });

	const glm::vec3 lightColor{ 1,1,1 };

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	const glm::mat4 rotationIncrement = glm::rotate(glm::mat4(1.0f), rotationSpeed, glm::normalize(glm::vec3(0, sinf(earthAxisOffset), cosf(earthAxisOffset))));

	while (!glfwWindowShouldClose(window))
	{
		// Update
		const float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		sunObject.getPosition() = { sunDistance * cosf(float(glfwGetTime())), 0, sunDistance * sinf(float(glfwGetTime())) };
		earthObject.getRotation() *= rotationIncrement;
		starsObject.getPosition() = camera.getPosition();

		camera.setSpeed(deltaTime * 5.0f);
		processInput(window, camera);

		// Draw
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		map.draw(earthObject.getRadius(), sunObject.getPosition() - earthObject.getPosition(), earthObject.getRotation());
		sunObject.draw(camera, lightColor, sunObject.getPosition());
		earthObject.draw(camera, lightColor, sunObject.getPosition());
		starsObject.draw(camera, lightColor, sunObject.getPosition());

		// Upkeep
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}