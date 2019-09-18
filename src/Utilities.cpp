#include "Utilities.h"

std::filesystem::path getShaderFolderPath()
{
	std::string root = std::filesystem::current_path().parent_path().string();
	root.append("/shader/");
	return std::filesystem::path(root);
}

std::filesystem::path getShaderPath(const std::string& shaderName)
{
	return std::filesystem::path(getShaderFolderPath().string() + "/" + shaderName);
}