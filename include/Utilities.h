#pragma once

#include "glm.hpp"

#include <iostream>

inline std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	out << v.x << " " << v.y << " " << v.z << "\n";
	return out;
}