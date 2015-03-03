#version 420

in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color.rgb = texture(tex, texCoord).rgb;
	color.a = 1.0;
}