#pragma once

#include "glm.hpp"
#include <glad/glad.h>

#include <iostream>
#include <filesystem>

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	out << v.x << " " << v.y << " " << v.z << "\n";
	return out;
}

struct CursorPosDescriptor
{
	float lastX;
	float lastY;

	static CursorPosDescriptor& instance()
	{
		static CursorPosDescriptor* ptr{ nullptr };

		if (!ptr)
		{
			ptr = new CursorPosDescriptor();
		}

		return *ptr;
	}

private:
	CursorPosDescriptor() = default;
};

struct ScreenDescriptor
{
	constexpr static int WINDOW_WIDTH = 1024;
	constexpr static int WINDOW_HEIGHT = 768;

private:
	ScreenDescriptor() = delete;
	ScreenDescriptor(const ScreenDescriptor& rhs) = delete;
	ScreenDescriptor(const ScreenDescriptor&& rhs) = delete;

	ScreenDescriptor& operator=(const ScreenDescriptor& rhs) = delete;
	ScreenDescriptor& operator=(const ScreenDescriptor&& rhs) = delete;
};

std::filesystem::path getShaderFolderPath();
std::filesystem::path getShaderPath(const std::string& shaderName);

std::filesystem::path getTextureFolderPath();
std::filesystem::path getTexturePath(const std::string& textureName);

GLuint loadTexture(const std::string& name, const int format);