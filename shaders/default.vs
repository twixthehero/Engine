#version 400

layout (location = 0) in vec3 inPosition;
//layout (location = 1) in vec2 inUV;
//layout (location = 2) in vec3 inNormal;

uniform mat4 mvp;

//out vec2 uv;
//out vec3 normal;

out vec3 col;

void main()
{
	gl_Position = mvp * vec4(inPosition, 1);
	col = inPosition;
	//uv = inUV;
	//normal = inNormal;
}