#version 420

in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;
uniform vec2 uPixelSize; // {pixelWidth / windowWidth, pixelHeight / windowHeight}

void main()
{
	vec2 coord = vec2(uPixelSize.x * floor(texCoord.x / uPixelSize.x), uPixelSize.y * floor(texCoord.y / uPixelSize.y));
	color.rgb = texture(tex, coord).rgb;
	color.a = 1.0;
}