#version 420

uniform sampler2D uBloom;
uniform sampler2D uScene;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	vec3 colorA = texture(uScene, texCoord).rgb;
	vec3 colorB = texture(uBloom, texCoord).rgb;

	outColor = 1.0f - (1.0f - colorA) * (1.0f - colorB);
}