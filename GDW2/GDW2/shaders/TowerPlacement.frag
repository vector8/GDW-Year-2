#version 420

in vec3 Position;
in vec2 texCoord;
in vec3 Normal;

out vec4 color;

uniform bool validPlacement;

void main()
{
	if(validPlacement)
	{
		color = vec4(0.0, 1.0, 0.0, 0.5);
	}
	else
	{
		color = vec4(1.0, 0.0, 0.0, 0.5);
	}
}