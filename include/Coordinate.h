#pragma once

#include <iostream>

#include "glm.hpp"

struct CoordinateXY
{
	float x;
	float y;

	void print() const
	{
		std::cout << "XY: " << x << " " << y << "\n";
	}
};

struct CoordinateLonLat
{
	float longitude;
	float latitude;

	void print() const
	{
		std::cout << "lonlat: " << longitude << " " << latitude << "\n";
	}

	glm::vec3 toDirectionVector() const
	{
		glm::vec3 ret;

		ret.x = cos(longitude) * cos(latitude);
		ret.y = sin(longitude) * cos(latitude);
		ret.z = sin(latitude);

		return ret;
	}
};

