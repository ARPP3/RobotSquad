#version 420

uniform sampler2D uTex; //source image
uniform float uThreshold;

in vec2 texCoord;

out vec3 outColor;

void main()
{
	vec3 color = texture(uTex, texCoord).rgb;

	float luminance = (color.r + color.g + color.b) / 3.0f;

	if (luminance > uThreshold)
	{
		outColor = color;
	}
	else
	{
		outColor = vec3(0.0f, 0.0f, 0.0f);
	}
}