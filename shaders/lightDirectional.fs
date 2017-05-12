#version 400

struct Light
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	Light light;
	vec3 direction;
};

uniform vec2 screenSize;
uniform sampler2D positionMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform DirectionalLight directionalLight;

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform vec3 eyeWorldPos;
uniform float shininess; //powers of 2

out vec4 glFragColor;

vec4 CalcLightInternal(vec3 matAlbedo, vec3 matSpecular, Light light, vec3 lightDirection, vec3 worldPos, vec3 normal)
{
	//ambient
	vec3 ambient = light.color * (0.1f * matAlbedo);

	//diffuse
	float diffuseFactor = max(dot(normal, -lightDirection), 0);
	vec3 diffuse = light.color * light.intensity * (diffuseFactor * matAlbedo);

	//specular
	vec3 vertexToEye = normalize(eyeWorldPos - worldPos);
	vec3 halfwayDir = normalize(-lightDirection + vertexToEye);
	float specularFactor = pow(max(dot(normal, halfwayDir), 0), shininess);
	vec3 specular = light.color * (specularFactor * matSpecular);

	return vec4(ambient + diffuse + specular, 1.0);
}

vec4 CalcDirectionalLight(vec3 matAlbedo, vec3 matSpecular, vec3 worldPos, vec3 normal)
{
	return CalcLightInternal(matAlbedo, matSpecular, directionalLight.light,
							 directionalLight.direction,
							 worldPos,
							 normal);
}

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 worldPos = texture(positionMap, texCoord).xyz;
	vec3 matAlbedo = texture(albedoMap, texCoord).xyz;
	vec3 normal = texture(normalMap, texCoord).xyz;
	normal = normalize(normal);
	vec3 matSpecular = texture(specularMap, texCoord).xyz;

	glFragColor = vec4(matAlbedo, 1) * CalcDirectionalLight(matAlbedo, matSpecular, worldPos, normal);
}