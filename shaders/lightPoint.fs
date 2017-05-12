#version 400

struct Light
{
	vec3 color;
	float intensity;
};

struct PointLight
{
	Light light;
	vec3 position;
	float range;
};

uniform vec2 screenSize;
uniform sampler2D positionMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform PointLight pointLight;

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

vec4 CalcPointLight(vec3 matAlbedo, vec3 matSpecular, vec3 worldPos, vec3 normal)
{
	vec3 lightDirection = worldPos - pointLight.position;
	float distance = length(lightDirection);
	lightDirection /= distance;

	vec4 color = CalcLightInternal(matAlbedo, matSpecular, pointLight.light, lightDirection, worldPos, normal);

	float modDistance = distance / pointLight.range;
	float atten = 1.0 / (1.0 + 25 * modDistance * modDistance);

	return color * atten;
}

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / screenSize;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 worldPos = texture(positionMap, texCoord).xyz;
	vec3 albedo = texture(albedoMap, texCoord).xyz;
	vec3 normal = texture(normalMap, texCoord).xyz;
	normal = normalize(normal);
	vec3 specular = texture(specularMap, texCoord).xyz;

	glFragColor = vec4(albedo, 1) * CalcPointLight(albedo, specular, worldPos, normal);
}