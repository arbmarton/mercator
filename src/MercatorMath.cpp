#include "MercatorMath.h"

namespace math
{

constexpr float radianToDegrees(const float radian)
{
	return radian * 180.0f / PI;
}

constexpr float degressToRadian(const float degrees)
{
	return degrees / 180.0f * PI;
}

}