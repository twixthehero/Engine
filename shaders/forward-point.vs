#version 400

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in mat4 modelMatrix;

uniform mat4 vp;

out vec3 position;
out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = vp * (modelMatrix * vec4(inPosition, 1));
	position = (modelMatrix * vec4(inPosition, 1)).xyz;
	uv = inUV;
	normal = (modelMatrix * vec4(inNormal, 0)).xyz;
}