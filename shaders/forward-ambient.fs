#version 400

in vec2 uv;

uniform sampler2D diffuse;

uniform vec4 ambientColor;
uniform float ambientIntensity;

out vec4 glFragColor;

void main()
{
	glFragColor = ambientColor * ambientIntensity * texture2D(diffuse, uv);
}