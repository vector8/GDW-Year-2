#version 330 core

in vec3 Position;
in vec2 UV;
in vec3 Normal;

out vec4 color;

uniform vec4 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D tex;

void main()
{
	vec3 ambientComponent = vec3(0.5f);
	vec3 diffuseComponent = vec3(0.5f);
	vec3 specularComponent = vec3(0.0f);
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - Position);
	
	float diffuse = max(dot(norm, lightDir), 0.0);
	diffuseComponent = diffuse * diffuseComponent;
	
	vec3 viewDir = normalize(viewPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	specularComponent = specular * specularComponent;

	vec3 result = (ambientComponent + diffuseComponent + specularComponent);
	color = vec4(result, 1.0f) * objectColor;
}