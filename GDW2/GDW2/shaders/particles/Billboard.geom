#version 420

in float size[];
in float alpha[];

// input type
layout (points) in;
// output type
layout (triangle_strip, max_vertices = 4) out;

// output data to fragment
out vec2 texCoord;
out float fragAlpha;

uniform mat4 projection;

void main()
{
	vec3 position = gl_in[0].gl_Position.xyz;

	fragAlpha = alpha[0];

	texCoord = vec2(0.0, 0.0);
	gl_Position = projection * vec4(position.xy + vec2(-0.5, -0.5) * size[0], position.z, 1.0);
	EmitVertex();

	texCoord = vec2(1.0, 0.0);
	gl_Position = projection * vec4(position.xy + vec2(0.5, -0.5) * size[0], position.z, 1.0);
	EmitVertex();

	texCoord = vec2(0.0, 1.0);
	gl_Position = projection * vec4(position.xy + vec2(-0.5, 0.5) * size[0], position.z, 1.0);
	EmitVertex();

	texCoord = vec2(1.0, 1.0);
	gl_Position = projection * vec4(position.xy + vec2(0.5, 0.5) * size[0], position.z, 1.0);
	EmitVertex();

	EndPrimitive();
}
