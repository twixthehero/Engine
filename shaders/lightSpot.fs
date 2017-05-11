#version 400

struct Light
{
	vec3 color;
	float intensity;
};

struct SpotLight
{
	Light light;
	vec3 position;
	vec3 direction;
	float angle;
	float cutoffAngle;
	float range;
};

uniform vec2 screenSize;
uniform sampler2D positionMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D specularMap;
uniform SpotLight spotLight;

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
	vec3 diffuse = light.color * (diffuseFactor * matAlbedo);

	//specular
	vec3 vertexToEye = normalize(eyeWorldPos - worldPos);
	vec3 lightReflect = normalize(reflect(lightDirection, normal));
	float specularFactor = pow(max(dot(vertexToEye, lightReflect), 0), shininess);
	vec3 specular = light.color * (specularFactor * matSpecular);

	return vec4((ambient + diffuse + specular) * matAlbedo, 1.0);
}

vec4 CalcSpotLight(vec3 matAlbedo, vec3 matSpecular, vec3 worldPos, vec3 normal)
{
	vec3 lightDirection = worldPos - spotLight.position;
	float distance = length(lightDirection);
	lightDirection /= distance;

	float theta = dot(spotLight.direction, lightDirection);
	float epsilon = spotLight.angle - spotLight.cutoffAngle;
	float intensity = max((theta - spotLight.cutoffAngle) / epsilon, 0);
	vec4 color = CalcLightInternal(matAlbedo, matSpecular, spotLight.light, lightDirection, worldPos, normal) * intensity;

	float atten = max(pow(cos(1.5 * spotLight.range), 3), 0);

	return color * spotLight.light.intensity * atten;
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

	glFragColor = vec4(albedo, 1) * CalcSpotLight(albedo, specular, worldPos, normal);
}