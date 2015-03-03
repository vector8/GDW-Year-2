#version 420

//uniform sampler2D sceneColor;
uniform sampler2D sceneDepth;

in vec2 texCoord;

out vec4 color;

void main()
{
	//float blend = exp(-texture(sceneDepth, texCoord).r * 0.001);

	//vec3 temp = mix(vec3(0.5, 0.5, 0.5), texture(sceneColor, texCoord).rgb, blend);

	float depth = texture(sceneDepth, texCoord).r;

	/*if(depth < 1.0 && depth > 0.99)
	{
		depth -= 0.999;

		depth *= 1000;

		if(depth < 0)
			depth = 0;
	}*/
	
	if (depth > 1.0)
	{
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (depth == 1.0)
	{
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}
	else if(depth > 0.99)
	{
		color = vec4(0.0, 1.0, 1.0, 1.0);
	}
	else if(depth < 0.0)
	{
		color = vec4(1.0, 0.0, 1.0, 1.0);
	}
	else
	{
		color = vec4(depth, depth, depth, 1.0);
	}
}