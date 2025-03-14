#version 420

uniform sampler2D uTex;
uniform float alpha = float(1.0);

in vec2 texCoord;

out vec4 outColor;

void main()
{
	vec4 texColor = texture2D(uTex, texCoord);
	outColor = texColor;
	outColor.a = outColor.a * alpha;
}