#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec3 Position;
out vec2 UV;
out vec3 Normal;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	Position = vec3(model * vec4(position, 1.0f));
	UV = uv;
	Normal = mat3(normalMatrix) * normal;
}