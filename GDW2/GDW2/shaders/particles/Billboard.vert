#version 420

uniform mat4 model;
uniform mat4 view;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inSize;
layout(location = 2) in float inAlpha;

out float size;
out float alpha;

void main()
{
	gl_Position = view * model * vec4(inPosition, 1.0);
	size = inSize;
	alpha = inAlpha;
}