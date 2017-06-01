#version 400

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

uniform mat4 vp;
uniform mat4 modelMatrix[250];

out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = vp * (modelMatrix[gl_InstanceID] * vec4(inPosition, 1.0));
	uv = inUV;
	normal = (modelMatrix[gl_InstanceID] * vec4(inNormal, 0)).xyz;
}