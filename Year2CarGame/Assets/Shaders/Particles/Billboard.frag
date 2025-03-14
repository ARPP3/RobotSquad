#version 420

uniform sampler2D uTex;

in vec2 texCoord;
in float frag_alpha;

out vec4 outColor;

void main()
{
	outColor = texture(uTex, texCoord).rgba;
	outColor.a *= frag_alpha;
}