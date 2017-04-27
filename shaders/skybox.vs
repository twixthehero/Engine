#version 400

layout (location = 0) in vec3 inPosition;

uniform mat4 projection;
uniform mat4 view;

out vec3 texCoords;

void main()
{
	vec4 pos = projection * view * vec4(inPosition, 1.0);
	gl_Position = pos.xyww;
	texCoords = inPosition;
}