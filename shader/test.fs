#version 330 core

out vec4 FragColor;

uniform float shade;

void main()
{
	FragColor = vec4(0.0f, shade, 0.0f, 1.0f);
}