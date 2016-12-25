#version 400

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

uniform mat4 mvp;

out vec3 position;
out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = mvp * vec4(inPosition, 1);
	position = inPosition;
	uv = inUV;
	normal = inNormal;
}