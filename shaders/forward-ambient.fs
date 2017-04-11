#version 400

in vec2 uv;

uniform sampler2D diffuse;

uniform vec3 ambientColor;
uniform float ambientIntensity;

out vec4 glFragColor;

void main()
{
	glFragColor = vec4(ambientColor * ambientIntensity, 1.0) * texture2D(diffuse, uv);
}