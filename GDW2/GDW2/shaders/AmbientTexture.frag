#version 420

in vec2 texCoord;

out vec4 color;

uniform vec4 objectColor;
uniform sampler2D tex;

void main()
{
	vec3 result = vec3(texture(tex, texCoord));
	color = vec4(result, 1.0f) * objectColor;
}