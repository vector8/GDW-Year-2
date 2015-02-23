#version 420

in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 source = texture(tex, texCoord);

	float luminance = 0.2989 * source.r + 0.587 * source.g + 0.114 * source.b;

	color = vec4(luminance, luminance, luminance, 1.0f);
}