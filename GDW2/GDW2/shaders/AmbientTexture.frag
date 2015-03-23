#version 420

in vec3 Position;
in vec2 texCoord;
in vec3 Normal;

out vec4 color;

uniform vec3 objectColor;
uniform sampler2D tex;

void main()
{
	vec3 result = texture(tex, texCoord).rgb;
	color = vec4(result * objectColor, 1.0f);
}