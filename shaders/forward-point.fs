#version 400

in vec3 position;
in vec2 uv;
in vec3 normal;

uniform sampler2D diffuse;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float range;

out vec4 glFragColor;

void main()
{
	vec3 lightDirection = lightPosition - position;
	float distance = length(lightDirection);
	lightDirection = normalize(lightDirection);
	float diffuseFactor = dot(normalize(normal), lightDirection);
	diffuseFactor = clamp(diffuseFactor, 0, 1);

	vec4 preAttenuationColor = vec4(lightColor * lightIntensity * diffuseFactor, 1.0) * texture(diffuse, uv);
	float attenuation = clamp(1.0 - distance * distance / (range * range), 0, 1);
	attenuation *= attenuation;

	glFragColor = preAttenuationColor / attenuation;
}