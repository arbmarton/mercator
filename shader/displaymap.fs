#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 gl_FragCoord;

uniform sampler2D ourTexture;
uniform vec2 upperLeftScreen;
uniform vec3 earthToSun;
uniform float earthRadius;

uniform float PI = 3.14159265358979323846264338327950f;

// Return value describes longitude and latitude euler angle
vec2 projectToSphere(vec2 v)
{
	vec2 ret;
	ret.x = /*lambdaNull +*/ v.x / earthRadius;
	ret.y = 2 * atan(exp(v.y / earthRadius)) - PI / 2;
	return ret;
}

vec3 toDirectionVector(vec2 v)
{
	vec3 ret;

	ret.x = cos(v.x) * cos(v.y);
	ret.z = sin(v.x) * cos(v.y);
	ret.y = sin(v.y);

	return ret;
}

void main()
{
	float currentX = (gl_FragCoord.x - upperLeftScreen.x) / (800.0f - upperLeftScreen.x);
	float currentY = gl_FragCoord.y / upperLeftScreen.y;

	currentX = (1.0f - currentX) * 2 * PI;
	currentY = currentY * PI - PI / 2;

	vec2 euler = projectToSphere(vec2(currentX, currentY));
	vec3 normal = toDirectionVector(euler);
	float result = dot(normal, earthToSun);

	if (result > 0)
	{
		FragColor = texture(ourTexture, TexCoord);
	}
	else
	{
		FragColor = texture(ourTexture, TexCoord) * 0.5f;
	}
	
}