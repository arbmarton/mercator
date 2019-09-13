#include "Converter.h"
#include "MercatorMath.h"

#include <math.h>

constexpr float lambdaNull = 0;

CoordinateXY Converter::ProjectToPlane(const CoordinateLonLat& lonlat, const float radius, const ProjectionMethod method)
{
	CoordinateXY ret;
	ret.x = radius * (lonlat.longitude - lambdaNull);
	ret.y = radius * float(log(float(tan(math::PI * 0.25f + lonlat.latitude * 0.5f))));
	return ret;
}

CoordinateLonLat Converter::ProjectToSphere(const CoordinateXY& xy, const float radius, const ProjectionMethod method)
{
	CoordinateLonLat ret;
	ret.longitude = lambdaNull + xy.x / radius;
	ret.latitude = 2 * atan(exp(xy.y / radius)) - math::PI_HALF;
	return ret;
}
