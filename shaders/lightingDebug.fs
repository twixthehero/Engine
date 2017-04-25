#version 400

uniform vec3 color;

out vec4 glFragColor;

void main()
{
	glFragColor = vec4(color, 1.0);
}