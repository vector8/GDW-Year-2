#version 420

uniform sampler2D uTex; // source image
uniform float uPixelSize; // 1.0 / windowWidth

in vec2 texCoord;

out vec3 outColor;

void main()
{
	// sample pixels in a horizontal row
	// the weights add up to 1
	outColor = vec3(0.0, 0.0, 0.0);

	outColor += 0.06 * texture(uTex, vec2(texCoord.x - 4.0 * uPixelSize, texCoord.y)).rgb;
	outColor += 0.09 * texture(uTex, vec2(texCoord.x - 3.0 * uPixelSize, texCoord.y)).rgb;
	outColor += 0.12 * texture(uTex, vec2(texCoord.x - 2.0 * uPixelSize, texCoord.y)).rgb;
	outColor += 0.15 * texture(uTex, vec2(texCoord.x - uPixelSize, texCoord.y)).rgb;
	outColor += 0.16 * texture(uTex, vec2(texCoord.x, texCoord.y)).rgb;
	outColor += 0.15 * texture(uTex, vec2(texCoord.x + uPixelSize, texCoord.y)).rgb;
	outColor += 0.12 * texture(uTex, vec2(texCoord.x + 2.0 * uPixelSize, texCoord.y)).rgb;
	outColor += 0.09 * texture(uTex, vec2(texCoord.x + 3.0 * uPixelSize, texCoord.y)).rgb;
	outColor += 0.06 * texture(uTex, vec2(texCoord.x + 4.0 * uPixelSize, texCoord.y)).rgb;
}