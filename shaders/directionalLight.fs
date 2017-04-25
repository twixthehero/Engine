#version 400

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec2 screenSize;

uniform sampler2D positions;
uniform sampler2D colors;
uniform sampler2D normals;

out vec4 glFragColor;

vec4 CalcDirectionalLight(vec3 worldPosition, vec3 normal)
{
	return vec4(0, 0, 0, 1);
}

vec2 CalcTextureCoordinate()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{
	vec2 uv = CalcTextureCoordinate();
	vec3 worldPosition = texture(positions, uv).xyz;
	vec3 color = texture(colors, uv).xyz;
	vec3 normal = texture(normals, uv).xyz;
	normal = normalize(normal);

	glFragColor = vec4(color, 1.0) * CalcDirectionalLight(worldPosition, normal);
}