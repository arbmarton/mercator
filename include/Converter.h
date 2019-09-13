#pragma once

#include "Coordinate.h"

enum class ProjectionMethod
{
	Mercator,
	WebMercator
};

class Converter
{
public:
	static CoordinateXY ProjectToPlane(const CoordinateLonLat& latlon, const float radius, const ProjectionMethod method = ProjectionMethod::Mercator);
	static CoordinateLonLat ProjectToSphere(const CoordinateXY& xy, const float radius, const ProjectionMethod method = ProjectionMethod::Mercator);

private:
	Converter() = default;
	~Converter() = default;
};

