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
uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform DirectionalLight directionalLight;

uniform vec3 ambientColor;
uniform float ambientIntensity;

uniform vec3 eyeWorldPos;
uniform float specularPower;
uniform float matSpecularIntensity;

out vec4 glFragColor;

vec4 CalcLightInternal(Light light, vec3 lightDirection, vec3 worldPos, vec3 normal)
{
	vec4 ambient = vec4(ambientColor * ambientIntensity, 1.0);
	float diffuseFactor = dot(normal, -lightDirection);

	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(light.color * light.intensity * diffuseFactor, 1.0);

		vec3 vertexToEye = normalize(eyeWorldPos - worldPos);
		vec3 lightReflect = normalize(reflect(lightDirection, normal));
		float specularFactor = dot(vertexToEye, lightReflect);

		if (specularFactor > 0)
		{
			specularFactor = pow(specularFactor, specularPower);
			specularColor = vec4(light.color * matSpecularIntensity * specularFactor, 1.0);
		}
	}

	return ambient + diffuseColor + specularColor;
}

vec4 CalcDirectionalLight(vec3 worldPos, vec3 normal)
{
	return CalcLightInternal(directionalLight.light,
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
	vec3 color = texture(colorMap, texCoord).xyz;
	vec3 normal = texture(normalMap, texCoord).xyz;
	normal = normalize(normal);

	glFragColor = vec4(color, 1) * CalcDirectionalLight(worldPos, normal);
}