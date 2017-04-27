#version 400

in vec3 texCoords;

uniform samplerCube skybox;

out vec4 glFragColor;

void main()
{
	glFragColor = texture(skybox, texCoords);
}