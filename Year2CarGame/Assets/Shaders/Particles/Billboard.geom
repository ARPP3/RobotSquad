#version 420

in float size[];
in float alpha[];

//Input type
layout (points) in;
//output type
layout (triangle_strip, max_vertices = 4) out;


out vec2 texCoord;
out float frag_alpha;

uniform mat4 uProj;

void main()
{
	vec3 position = gl_in[0].gl_Position.xyz;

	frag_alpha = alpha[0];
	texCoord = vec2(0.0, 0.0);
	gl_Position = uProj * vec4(position.xy + vec2(-0.5, -0.5) * size[0], position.z, 1.0);
	EmitVertex();

	frag_alpha = alpha[0];
	texCoord = vec2(1.0, 0.0);
	gl_Position = uProj * vec4(position.xy + vec2(0.5, -0.5) * size[0], position.z, 1.0);
	EmitVertex();
	
	frag_alpha = alpha[0];
	texCoord = vec2(0.0, 1.0);
	gl_Position = uProj * vec4(position.xy + vec2(-0.5, 0.5) * size[0], position.z, 1.0);
	EmitVertex();

	frag_alpha = alpha[0];
	texCoord = vec2(1.0, 1.0);
	gl_Position = uProj * vec4(position.xy + vec2(0.5, 0.5) * size[0], position.z, 1.0);
	EmitVertex();

	EndPrimitive();
}