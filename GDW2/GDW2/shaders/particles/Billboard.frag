#version 420

uniform sampler2D tex;

in vec2 texCoord;
in float fragAlpha;

out vec4 color;

void main()
{
	color = texture(tex, texCoord);
	color.a *= fragAlpha;
}