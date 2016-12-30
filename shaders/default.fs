#version 400

uniform vec4 ambientColor;
uniform float ambientIntensity;

in vec2 uv;

uniform sampler2D diffuse;

out vec4 glFragColor;

void main()
{
	glFragColor = texture2D(diffuse, uv).rgba;// * (ambientColor * ambientIntensity);
}