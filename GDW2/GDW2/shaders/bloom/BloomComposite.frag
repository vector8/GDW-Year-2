#version 420

uniform sampler2D uScene;
uniform sampler2D uBloom;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	vec3 colorA = texture(uScene, texCoord).rgb;
	vec3 colorB = texture(uBloom, texCoord).rgb;
	outColor = 1.0 - (1.0 - colorA) * (1.0 - colorB);
}