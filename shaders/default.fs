#version 400

//uniform vec4 color;

//in vec2 uv;
//in vec3 normal;

in vec3 col;

//out vec4 glFragColor;

void main()
{
	gl_FragColor = vec4(col, 1.0);
	//gl_FragColor = vec4(1.0, 0.5, 0.0, 1.0);
	//glFragColor = color;
}