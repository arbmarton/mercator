#pragma once

#include "glm.hpp"

#include <iostream>
#include <filesystem>

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	out << v.x << " " << v.y << " " << v.z << "\n";
	return out;
}

std::filesystem::path getShaderFolderPath();
std::filesystem::path getShaderPath(const std::string& shaderName);