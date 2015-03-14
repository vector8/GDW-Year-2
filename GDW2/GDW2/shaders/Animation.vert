#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 position2;
layout (location = 4) in vec3 normal2;

out vec3 Position;
out vec2 texCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float blend;

void main()
{
	vec3 newPosition = mix(position, position2, blend);
	vec3 newNormal = mix(normal, normal2, blend);

	gl_Position = projection * view * model * vec4(newPosition, 1.0f);
	Position = vec3(model * vec4(newPosition, 1.0f));
	texCoord = uv;
	Normal = mat3(transpose(inverse(model))) * newNormal;
}