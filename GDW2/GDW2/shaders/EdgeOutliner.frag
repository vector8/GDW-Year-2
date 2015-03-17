#version 420

uniform sampler2D scene; // source image
uniform sampler2D sceneNormals; 
uniform sampler2D sceneDepth;
uniform vec2 uPixelSize; // {1.0 / windowWidth, 1.0 / windowHeight}

in vec2 texCoord;

out vec4 outColor;

float sobelHoriz(sampler2D image);
float sobelVert(sampler2D image);

void main()
{
	vec3 color = texture(scene, texCoord).rgb;

	float normalHoriz = sobelHoriz(sceneNormals);
	float normalVert = sobelVert(sceneNormals);
	float depthHoriz = sobelHoriz(sceneDepth);
	float depthVert = sobelVert(sceneDepth);

	float edge = normalHoriz * normalVert * depthHoriz * depthVert;

	outColor = vec4(edge * color, 1.0);
}

float sobelHoriz(sampler2D image)
{
	vec2 offset[6] = vec2[](vec2(-uPixelSize.x, -uPixelSize.y), vec2(uPixelSize.x, -uPixelSize.y),
							vec2(-uPixelSize.x, 0.0), vec2(uPixelSize.x, 0.0),
							vec2(-uPixelSize.x, uPixelSize.y), vec2(uPixelSize.x, uPixelSize.y));

	vec3 sum = vec3(0.0);

	sum += -texture(image, offset[0] + texCoord).rgb;
	sum += texture(image, offset[1] + texCoord).rgb;
	sum += -2.0 * texture(image, offset[2] + texCoord).rgb;
	sum += 2.0 * texture(image, offset[3] + texCoord).rgb;
	sum += -texture(image, offset[4] + texCoord).rgb;
	sum += texture(image, offset[5] + texCoord).rgb;

	float lengthSquared = dot(sum, sum);
	return (lengthSquared < 1.0 ? 1.0 : 0.0);
}

float sobelVert(sampler2D image)
{
	vec2 offset[6] = vec2[](vec2(-uPixelSize.x, -uPixelSize.y), vec2(0.0, -uPixelSize.y), vec2(uPixelSize.x, -uPixelSize.y),
							vec2(-uPixelSize.x, uPixelSize.y), vec2(0.0, uPixelSize.y), vec2(uPixelSize.x, uPixelSize.y));

	vec3 sum = vec3(0.0);

	sum += -texture(image, offset[0] + texCoord).rgb;
	sum += -2.0 * texture(image, offset[1] + texCoord).rgb;
	sum += -texture(image, offset[2] + texCoord).rgb;
	sum += texture(image, offset[3] + texCoord).rgb;
	sum += 2.0 * texture(image, offset[4] + texCoord).rgb;
	sum += texture(image, offset[5] + texCoord).rgb;

	float lengthSquared = dot(sum, sum);
	return (lengthSquared < 1.0 ? 1.0 : 0.0);
}