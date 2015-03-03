#version 420

uniform sampler2D scene;
uniform sampler2D bloom;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	vec3 sceneColor = texture(scene, texCoord).rgb;
	vec3 bloomColor = texture(bloom, texCoord).rgb;
	outColor = 1.0 - (1.0 - sceneColor) * (1.0 - bloomColor);
}