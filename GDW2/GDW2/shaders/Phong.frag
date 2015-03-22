#version 420

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float specularExponent;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

#define NUM_POINT_LIGHTS 1

in vec3 Position;
in vec2 texCoord;
in vec3 Normal;

out vec4 color;

uniform Material material;
uniform DirectionalLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];

uniform vec3 objectColor;

uniform float fogFactor;

uniform vec3 viewPos;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - Position);
	
	vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		result += calculatePointLight(pointLights[i], norm, Position, viewDir);
	}

	// Apply fog effect.
	float blend = exp(-length(viewPos - Position) * fogFactor);
	result = mix(vec3(0.5, 0.5, 0.5), result.rgb, blend);

	color = vec4(result * objectColor, 1.0f);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoord));

	return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir)
{
	// Attenuation
	vec3 lightVec = light.position - fragmentPos;
	float distance = length(lightVec);
	vec3 lightDir = lightVec / distance;
	float attenuation = 1.0 / (light.constantAttenuation + (light.linearAttenuation * distance) + (light.quadraticAttenuation * distance * distance));
	
	// Ambient
	vec3 ambient = attenuation * light.ambient * vec3(texture(material.diffuse, texCoord));

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * attenuation * light.diffuse * vec3(texture(material.diffuse, texCoord));

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);
	vec3 specular = spec * attenuation * light.specular * vec3(texture(material.specular, texCoord));

	return ambient + diffuse + specular;
}