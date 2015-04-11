#version 420

in vec3 Position;
in vec2 texCoord;
in vec3 Normal;

out vec4 color;

uniform mat4 worldToShadowMap;
uniform sampler2D shadowMapDepth;
uniform bool drawShadow;

void main()
{
	vec3 result = vec3(1.0);

	if(drawShadow)
	{
		// Determine if this fragment is in shadow.
		vec4 shadowCoord = worldToShadowMap * vec4(Position, 1.0);
		float shadowDepth = texture(shadowMapDepth, shadowCoord.xy).r;

		if(shadowDepth < shadowCoord.z - 0.001)
		{
			// The fragment is in shadow.
			result *= 0.5;
		}
	}

	color = vec4(result, 1.0);
}