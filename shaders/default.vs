#version 400

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 mvp;

out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = mvp * vec4(inPosition, 1);

	vec3 positionCamera = (V * M * vec4(inPosition, 1)).xyz;
	vec3 eyeDirectionCamera = vec3(0, 0, 0) - positionCamera;

	vec3 lightPositionCamera = (V * vec4(0, 0, 0, 1)).xyz;
	vec3 lightDirectionCamera = lightPositionCamera + eyeDirectionCamera;
	
	vec3 normalCamera = (V * M * vec4(inNormal, 0)).xyz;

	vec3 n = normalize(normalCamera);
	vec3 l = normalize(lightDirectionCamera);

	uv = inUV;
	normal = inNormal;
}