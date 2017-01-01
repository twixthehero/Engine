#version 400

uniform float intensity;
uniform vec4 color;

uniform vec2 screenSize;

uniform sampler2D colors;

out vec4 glFragColor;

vec2 CalcTextureCoordinate()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{
	glFragColor = color * intensity * texture2D(colors, CalcTextureCoordinate());
}