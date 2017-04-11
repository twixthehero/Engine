#version 400

uniform float intensity;
uniform vec3 color;

uniform vec2 screenSize;

uniform sampler2D colors;

out vec4 glFragColor;

vec2 CalcTextureCoordinate()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{
	glFragColor = vec4(color * intensity, 1.0) * texture2D(colors, CalcTextureCoordinate());
}