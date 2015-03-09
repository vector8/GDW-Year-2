#version 420

uniform sampler2D scene;
uniform sampler2D shadows;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	vec3 sceneColor = texture(scene, texCoord).rgb;
	vec3 shadowColor = texture(shadows, texCoord).rgb;
	outColor = sceneColor * shadowColor;
}