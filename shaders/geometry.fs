#version 400

in vec2 uv;
in vec3 normal;
in vec3 position;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outDiffuse;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outUV;

uniform sampler2D diffuse;

void main()
{
	outPosition = position;
	outDiffuse = texture2D(diffuse, uv).xyz;
	outNormal = normalize(normal);
	outUV = vec3(uv, 0.0);
}