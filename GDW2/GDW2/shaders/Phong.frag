#version 420

in vec3 Position;
in vec2 texCoord;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 lightColour;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float specularExponent;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform sampler2D tex;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightVec = lightPos - Position;
	float distance = length(lightVec);
	vec3 lightDir = lightVec / distance;

	float attenuation = 1.0 / (constantAttenuation + (linearAttenuation * distance) + (quadraticAttenuation * distance * distance));
	
	float d = max(dot(norm, lightDir), 0.0);
	vec3 diffuseComponent = d * diffuse * attenuation * lightColour;
	
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float s = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
	vec3 specularComponent = s * specular * attenuation * lightColour;

	vec3 result = (ambient + diffuseComponent + specularComponent);
	color = vec4(result, 1.0f) * texture(tex, texCoord);
}