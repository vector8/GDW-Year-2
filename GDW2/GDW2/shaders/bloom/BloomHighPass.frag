#version 420

uniform sampler2D scene; // source image
uniform float threshold;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	// Sample the scene for the current fragment.
	vec3 color = texture(scene, texCoord).rgb;

	// Calculate the luminance of the sample.
	float luminance = (color.r + color.g + color.b) / 3.0;

	if(luminance > threshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0.0, 0.0, 0.0);
	}
}