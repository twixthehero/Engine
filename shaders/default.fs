#version 400

in vec2 uv;
in vec3 normal;

in vec3 col;

uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture2D(diffuse, uv).rgba;
}