#version 400

in vec2 uv;
in vec3 normal;

uniform sampler2D diffuse;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform float lightIntensity;

out vec4 glFragColor;

void main()
{
	float diffuseFactor = dot(normalize(normal), -lightDirection);
	diffuseFactor = clamp(diffuseFactor, 0, 1);

	glFragColor = vec4(lightColor * lightIntensity * diffuseFactor, 1.0) * texture2D(diffuse, uv);
}