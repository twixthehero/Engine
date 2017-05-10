#version 400

in vec2 uv;
in vec3 normal;
in vec3 position;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outAlbedo;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outSpecular;

uniform sampler2D albedo;
uniform vec3 albedoColor;
uniform sampler2D specular;
uniform vec3 specularColor;

void main()
{
	outPosition = position;
	outAlbedo = texture(albedo, uv).xyz * albedoColor;
	outNormal = normalize(normal);
	outSpecular = texture(specular, uv).xyz * specularColor;
}