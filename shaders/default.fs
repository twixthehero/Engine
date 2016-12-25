#version 400

uniform vec4 ambientColor;
uniform float ambientIntensity;

in vec2 uv;

uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture2D(diffuse, uv).rgba * (ambientColor * ambientIntensity);
}